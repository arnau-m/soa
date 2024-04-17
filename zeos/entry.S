/*
 * entry.S - Entry point to system mode from user mode
 */

#include <asm.h>
#include <segment.h>
#include <errno.h>


/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %edx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %ebx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/

#define SAVE_ALL \
      pushl %gs; \
      pushl %fs; \
      pushl %es; \
      pushl %ds; \
      pushl %eax; \
      pushl %ebp; \
      pushl %edi; \
      pushl %esi; \
      pushl %ebx; \
      pushl %ecx; \
      pushl %edx; \
      movl $__KERNEL_DS, %edx;    \
      movl %edx, %ds;           \
      movl %edx, %es
      
#define RESTORE_ALL \
      pop %edx; \
      pop %ecx; \
      pop %ebx; \
      pop %esi; \
      pop %edi; \
      pop %ebp; \
      pop %eax; \
      pop %ds; \
      pop %es; \
      pop %fs; \
      pop %gs; 
      
#define EOI \
	  movb $0x20, %al ; \
	  outb %al, $0x20 ;
      

ENTRY(clock_handler) 
	  SAVE_ALL 
	  EOI
	  call clock_routine
	  RESTORE_ALL
	  iret
	  
	  
ENTRY(keyboard_handler) 
	  SAVE_ALL 
	  call keyboard_routine
	  EOI
	  RESTORE_ALL
	  iret
	  
ENTRY(p_f_handler)
	  call p_f_routine

ENTRY (writeMSR)
        push %ebp
        mov %esp, %ebp

        mov 0x8(%ebp), %ecx
        movl $0, %edx
        movl 12(%ebp), %eax
        wrmsr

        popl %ebp
        ret

ENTRY(syscall_handler_sysenter)
        push $__USER_DS
        push %ebp // User stack address
        pushfl
        push $__USER_CS
        push 4(%ebp) // User return address
        SAVE_ALL
        cmpl $0, %eax
        jl sysenter_err
        cmpl $MAX_SYSCALL, %eax
        jg sysenter_err
        call *sys_call_table(, %eax, 0x04)
        jmp sysenter_fin
  sysenter_err:
        movl $-38, %eax
  sysenter_fin:
        movl %eax, 0x18(%esp)
        RESTORE_ALL
        movl (%esp), %edx // Return address
        movl 12(%esp), %ecx // User stack address
        sti // Enable interrupts again
        sysexit
        
ENTRY(task_switch)
      pushl %ebp          // Guardem a la pila pila el punter a la base de la pila
      movl %esp, %ebp     // Fem que el punter de la base de la pila apunti al top de la pila

      pushl %ebx          //Guardem registre base per direccionament de memoria       
      pushl %esi          //Guardem registre Source Index register
      pushl %edi          //Guardem registre Destination Index

      push 8(%ebp)        //Agafem primer paràmetre (punter nou task_union)

      call inner_task_switch //Un cop guardats els paràmetres cridem a la funció en c

      addl $4, %esp       //Restaurem el %esp
      popl %edi           //Restaurem edi
      popl %esi           //Restaurem esi
      popl %ebx           //Restaurem ebx

      movl %ebp, %esp     //Restaurem  esp i retornem
      popl %ebp           
      ret

ENTRY(ebp)
      movl %ebp, %eax
      ret

ENTRY(canvi_esp)
      movl 4(%esp), %esp
      popl %ebp
      ret

