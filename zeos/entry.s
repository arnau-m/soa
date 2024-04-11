# 0 "entry.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "entry.S"




# 1 "include/asm.h" 1
# 6 "entry.S" 2
# 1 "include/segment.h" 1
# 7 "entry.S" 2
# 1 "include/errno.h" 1
# 8 "entry.S" 2
# 73 "entry.S"
.globl clock_handler; .type clock_handler, @function; .align 0; clock_handler:
   pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
   movb $0x20, %al ; outb %al, $0x20 ;
   call clock_routine
   pop %edx; pop %ecx; pop %ebx; pop %esi; pop %edi; pop %ebp; pop %eax; pop %ds; pop %es; pop %fs; pop %gs;
   iret


.globl keyboard_handler; .type keyboard_handler, @function; .align 0; keyboard_handler:
   pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
   call keyboard_routine
   movb $0x20, %al ; outb %al, $0x20 ;
   pop %edx; pop %ecx; pop %ebx; pop %esi; pop %edi; pop %ebp; pop %eax; pop %ds; pop %es; pop %fs; pop %gs;
   iret

.globl p_f_handler; .type p_f_handler, @function; .align 0; p_f_handler:
   call p_f_routine

.globl writeMSR; .type writeMSR, @function; .align 0; writeMSR:
        push %ebp
        mov %esp, %ebp

        mov 0x8(%ebp), %ecx
        movl $0, %edx
        movl 12(%ebp), %eax
        wrmsr

        popl %ebp
        ret

.globl syscall_handler_sysenter; .type syscall_handler_sysenter, @function; .align 0; syscall_handler_sysenter:
        push $0x2B
        push %ebp
        pushfl
        push $0x23
        push 4(%ebp)
        pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
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
        pop %edx; pop %ecx; pop %ebx; pop %esi; pop %edi; pop %ebp; pop %eax; pop %ds; pop %es; pop %fs; pop %gs;
        movl (%esp), %edx
        movl 12(%esp), %ecx
        sti
        sysexit

.globl task_switch; .type task_switch, @function; .align 0; task_switch:
      pushl %ebp
      movl %esp, %ebp

      pushl %ebx
      pushl %esi
      pushl %edi

      push 8(%ebp)

      call inner_task_switch

      addl $4, %esp
      popl %edi
      popl %esi
      popl %ebx

      movl %ebp, %esp
      popl %ebp
      ret

.globl ebp; .type ebp, @function; .align 0; ebp:
      movl %ebp, %eax
      ret

.globl canvi_esp; .type canvi_esp, @function; .align 0; canvi_esp:
      movl 4(%esp), %esp
      popl %ebp
      ret
