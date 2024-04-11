# 0 "wrapper.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "wrapper.S"




# 1 "include/asm.h" 1
# 6 "wrapper.S" 2

.globl write; .type write, @function; .align 0; write:
    pushl %ebp
    movl %esp, %ebp

    pushl %edx
    pushl %ecx
    pushl %ebx

    movl 8(%ebp), %edx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %ebx
    movl $4, %eax

    pushl %ecx
    pushl %edx
    pushl $write_ret
    pushl %ebp
    movl %esp, %ebp
    sysenter

write_ret:

    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    cmpl $0, %eax
    jl write_error

    popl %ebx
    popl %ecx
    popl %edx

    popl %ebp
    ret

write_error:
    negl %eax
    movl %eax, errno
    movl $-1, %eax

    popl %ebx
    popl %ecx
    popl %edx

    popl %ebp
    ret


.globl gettime; .type gettime, @function; .align 0; gettime:
    pushl %ebp
    movl %esp, %ebp


    movl $10, %eax

    pushl %ecx
    pushl %edx
    pushl $gettime_ret
    pushl %ebp
    movl %esp, %ebp
    sysenter

gettime_ret:
    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    cmpl $0, %eax
    jl gettime_error

    popl %ebp
    ret

gettime_error:
    negl %eax
    movl %eax, errno
    movl $-1, %eax

    popl %ebp
    ret

.globl getpid; .type getpid, @function; .align 0; getpid:
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
