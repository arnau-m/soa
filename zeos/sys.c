/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

extern int zeos_ticks;

char buff[1024];

int check_fd(int fd, int permissions)
{
	if (fd != 1)
		return -9; /*EBADF*/
	if (permissions != ESCRIPTURA)
		return -13; /*EACCES*/
	return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
	int PID = -1;

	// a. Obtenir task_struc lliure de la freequeue, sino retornar error.
	if (list_empty(&freequeue))
	{
		return -28;
	}
	struct list_head *free = list_first(&freequeue);
	list_del(free);

	// b. Copiar task_union del pare al fill
	union task_union *fill = (union task_union *)list_head_to_task_struct(free);					  // task_union del fill
	union task_union *pare = (union task_union *)current();											  // task_union del pare
	copy_data((void *)&pare->stack, (void *)&fill->stack, KERNEL_STACK_SIZE * sizeof(unsigned long)); // Copiem segmento kernel (igual fill i pare)

	// c. Directori de pagines on guardarem l'espai d'adreces del fill
	allocate_DIR((struct task_struc *)fill); // Allocatem memoria logica pel fill

	// d. Busquem pagines fisiques on mapejar les logiguqes per data+stack, si no hi ha prou pag retornar error
	page_table_entry *PT_fill = get_PT(&fill->task); // obtenim punter a la taula de pagines del fill
	int pagines[NUM_PAG_DATA];						 // Creem un vector per guardar pagines allocatades i en cas de no haver prou espai desallocatarles
	for (int i = 0; i < NUM_PAG_DATA; ++i)
	{
		pagines[i] = alloc_frame(); // Allocatem pagina i guardem resultat al vector
		if (pagines[i] < 0)
		{ // En cas de no haver prou pagines, desallocatem les allocatades, treiem pcb de la freequeue i retornem error
			for (int j = 0; j < i; ++j)
			{
				free_frame(pagines[j]);
			}
			list_add_tail(PT_fill, &freequeue);
			return -ENOMEM;
		}
	}

	// e. Inicialització de l'espai d'adreçes del fill mapejant adreçes físiques amb logiques
	page_table_entry *PT_pare = get_PT(current()); // Punter taula de pagines del pare

	// i) Mapejem codi sistema , dades sistema i codi usuari compartits
	for (int i = 0; i < NUM_PAG_KERNEL; i++)
	{
		set_ss_pag(PT_fill, i, get_frame(PT_pare, i)); // Mapejem frames logics de kernel del fill amb els fisics del pare
	}

	for (int i = 0; i < NUM_PAG_CODE; i++)
	{
		set_ss_pag(PT_fill, PAG_LOG_INIT_CODE + i, get_frame(PT_pare, PAG_LOG_INIT_CODE + i)); // Mapejem frames logics de codi del fill amb els fisics del pare
	}

	// ii) Mapejem dades i pila usuari a nous frames fisics allocatats anteriorment
	for (int i = 0; i < NUM_PAG_DATA; i++)
	{
		set_ss_pag(PT_fill, PAG_LOG_INIT_DATA + i, pagines[i]); // Mapejem als frames alocatats anteriorment
	}

	//f. Copiem dades d'usuari mapejant pagines logiques a les fisiques del fill per fer la copia des del pare

	int compartit = NUM_PAG_CODE + NUM_PAG_KERNEL;						//pagina on acaba l'espai compartit
	for (int i = compartit; i < compartit + NUM_PAG_DATA; i++)			//iterem sobre les pagines no compartides
	{
		set_ss_pag(PT_pare, NUM_PAG_DATA + i, get_frame(PT_fill, i));	//Mapejem pagines logiques lliures del pare amb les fisiques del fill
		copy_data((void *)(i << 12), (void *)((i + NUM_PAG_DATA) << 12), PAGE_SIZE);	//Copiem dades del pare al fill
		del_ss_pag(PT_pare, i + NUM_PAG_DATA);	//Desmapejem les pagines logiques del pare amb les fisiques del fill
	}
	set_cr3(get_DIR(current()));	//Provoquem flush de TLB per eliminar accessos del pare al fill de TLB

	


	return PID;
}

void sys_exit()
{
}

int sys_write(int fd, char *buffer, int size)
{
	int rev = check_fd(fd, ESCRIPTURA);
	if (rev < 0)
		return rev;

	if (size < 0)
		return EINVAL;

	if (buffer == NULL)
		return EFAULT;
	if (!access_ok(0, buffer, size))
		return EFAULT;

	int i = 0;
	while ((i + 1024) <= size)
	{
		rev = copy_from_user(buffer + i, buff, 1024);
		if (rev < 0)
			return rev;
		sys_write_console(buff, 1024);
		i += 1024;
	}
	if ((size - i) > 0)
	{
		rev = copy_from_user(buffer + i, buff, size - i);
		if (rev < 0)
			return rev;
		sys_write_console(buff, size - i);
	}

	return size;
}

int sys_gettime()
{
	return zeos_ticks;
}
