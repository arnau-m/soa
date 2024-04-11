/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <interrupt.h>


//Array on tindrem els 10 processos. 
//La part del __atribute__ serveix per indicar que volem que la variable 'task' es guardi a la secció de memoria '.data.task'
//El proces a task[0] sera l'idle (entra a CPU quan no hi ha res més a fer) i no es pot destruir
union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));


struct task_struct *list_head_to_task_struct(struct list_head *l)
{
	struct task_struct * ts = (struct task_struct *) ((unsigned long)l & 0xfffff000);
  	return ts;
}

extern struct list_head blocked;
struct list_head freequeue;
struct list_head readyqueue;
struct task_struct * idle_task;

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

/*
	Funció a executar a CPU pel procés idle quan no hi ha res més a executar
*/
void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	printk("Entrem al proces IDLE");
	
	while(1)
	{
	;
	}
}

/*
	Inicialització procés IDLE
*/
void init_idle (void)
{	

	//Obtenim el primer list_head lliure de la freequeue
	struct list_head *first = list_first(&freequeue);
	
	//Com que el farem servir l'eliminem de la freequeeu
	list_del(first); 

	//Convertim el list_head a task_struct
	struct task_struct *pcb = list_head_to_task_struct(first);

	//Assignem el PID 0 al procés IDLE
	pcb->PID = 0;

	//Inicialitzem dir_pages_baseAddr amb un nou directori per guardar l'espai d'adreçes
	allocate_DIR(pcb);

	/*
	Ara inicialitzem context d'execució per poder restaurar quan idle s'assigni a la CPU
	No cal guardar ctxHW ni ctxSW, només info pk el task_switch pugui fer canvi: ebp i @ret
	*/
	//Obtenim task_union a partir del task_struct (comparteixen @ inicial)
	union task_union *tu = (union task_union*)pcb; 

	tu -> stack[KERNEL_STACK_SIZE - 1] =  (unsigned long) cpu_idle; //A la penultima posició de la pila guardem l'adreça per retornar a la funció cpu_idle
	tu -> stack[KERNEL_STACK_SIZE - 2] = (unsigned long) 0;		   //ebp, pot ser 0
	tu -> task.kernel_esp = &(tu -> stack[KERNEL_STACK_SIZE - 2]);

	idle_task = pcb;
	
	
}

void init_task1(void)
{
	
	//Obtenim un task_union disponible de la freequeue
	struct list_head *first = list_first(&freequeue);
		
	//Com que l'hem fet servir, l'hem d'eliminar de la freequeue
	list_del(first); 

	//Convertim el list_head a task_struct
	struct task_struct *pcb = list_head_to_task_struct(first);
	
	//Assignem el PID 1 al procés
	pcb->PID = 1;

	//Allocatem nou directori per al proces
	allocate_DIR(pcb);
	//Completem inicialització allocatant pagines fisiques per ubicar l'espai d'adreçes d'usuari i afegeix traducció log-fiq
	set_user_pages(pcb);

	//Actualitzem TSS per apuntar a la nova tasca
	union task_union *tu = (union task_union *) pcb;	//Convertim task_struct (pcb) a task_union (tu)
	tss.esp0 = KERNEL_ESP(tu);							//Fem que esp0 apunti a inici codi usuari
	writeMSR(0x175, (int) tss.esp0);					//Modifiquem MSR per fer servir sysenter

	//Fem que la pagina del directori del procés sigui la pàgina del directori actual en el sistema
	set_cr3(pcb->dir_pages_baseAddr);
	
}


void init_sched()
{
	INIT_LIST_HEAD(&readyqueue);
	INIT_LIST_HEAD(&freequeue);
	for (int i = 0; i < NR_TASKS; ++i) {
		list_add(&task[i].task.list, &freequeue); //Abans teniem list_add_tail, pero crec que s'ha de fer servir aquest
	}
}

void inner_task_switch(union task_union* new){

	/*
	1) Update the pointer to the system stack to point to the stack of new_task. This step depends
	on the implemented mechanism to enter the system. In the case that the int assembly
	instruction is used to invoke the system code, TSS.esp0 must be modified to make it point
	to the stack of new_task. If the system code is invoked using sysenter, MSR number 0x175
	must be also modified*/
	tss.esp0 = KERNEL_ESP(new);		
	writeMSR(0x175, tss.esp0);
	/*
	2) Change the user address space by updating the current page directory: use the set_cr3
	funtion to set the cr3 register to point to the page directory of the new_task*/
	set_cr3(get_DIR((struct task_struct*)new));
	/*
	3) Store the current value of the EBP register in the PCB. EBP has the address of the current
	system stack where the inner_task_switch routine begins (the dynamic link).*/
	current()->kernel_esp = ebp();

	/*4) Change the current system stack by setting ESP register to point to the stored value in the
	new PCB.*/
	canvi_esp(new->task.kernel_esp);
	
}	

//Retorna l'adreça del task_struct actual
struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

