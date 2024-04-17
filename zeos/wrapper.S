/*
 * wrapper.s - Entry point to system mode from user mode
 */

#include <asm.h>

ENTRY(write)
    pushl %ebp
    movl %esp, %ebp

    pushl %edx
    pushl %ecx
    pushl %ebx
    
    movl 8(%ebp), %edx     // Move file descriptor to %edx
    movl 12(%ebp), %ecx    // Move buffer address to %ecx
    movl 16(%ebp), %ebx    // Move size to %ebx
    movl $4, %eax          // System call number for write

    pushl %ecx
    pushl %edx
    pushl $write_ret
    pushl %ebp
    movl %esp, %ebp
    sysenter               // Make the system call

write_ret:

    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    cmpl $0, %eax          // Check if there was an error
    jl  write_error        // Jump to write_error if there was an error

    popl %ebx
    popl %ecx
    popl %edx 

    popl %ebp              // Restore the stack frame
    ret

write_error:
    negl %eax              // Negate the error code
    movl %eax, errno       // Store the error code in errno
    movl $-1, %eax         // Set return value to -1

    popl %ebx
    popl %ecx
    popl %edx 
    
    popl %ebp              // Restore the stack frame
    ret


ENTRY(gettime)
    pushl %ebp
    movl %esp, %ebp

    
    movl $10, %eax          // System call number for write

    pushl %ecx
    pushl %edx
    pushl $gettime_ret
    pushl %ebp
    movl %esp, %ebp
    sysenter               // Make the system call
gettime_ret:
    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    cmpl $0, %eax          // Check if there was an error
    jl  gettime_error        // Jump to write_error if there was an error

    popl %ebp              // Restore the stack frame
    ret
gettime_error:
    negl %eax              // Negate the error code
    movl %eax, errno       // Store the error code in errno
    movl $-1, %eax         // Set return value to -1

    popl %ebp              // Restore the stack frame
    ret

ENTRY(getpid)
    pushl %ebp
    movl %esp, %ebp

    movl $20, %eax

    pushl $ret_getpid
    pushl %ebp
    movl %esp, %ebp

    sysenter

ret_getpid:
    popl %ebp
    addl $4, %esp
    
    cmpl $0, %eax
    jge getpid_no_error
    negl %eax
    movl %eax, errno
    movl $-1, %eax

getpid_no_error:
    popl %ebp
    ret

ENTRY(fork)
    pushl %ebp
    movl %esp, %ebp

    movl $2, %eax

    pushl $ret_fork
    pushl %ebp
    movl %esp, %ebp

    sysenter

ret_fork:
    popl %ebp
    addl $4, %esp

    cmpl $0, %eax
    jge fork_no_error
    negl %eax
    movl %eax, errno
    movl $-1, %eax

fork_no_error:
    popl %ebp
    ret