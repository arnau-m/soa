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
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
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
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}

int sys_write(int fd, char * buffer, int size){
	int rev = check_fd(fd, ESCRIPTURA);
	if (rev < 0) return rev;
	
	if (size < 0) return EINVAL;

	if (buffer == NULL) return EFAULT;
	if (!access_ok(0,buffer,size)) return EFAULT;

	int i = 0;
	while ((i + 1024) <= size) {
		rev = copy_from_user(buffer+i, buff, 1024);
		if(rev < 0) return rev;
		sys_write_console(buff, 1024);
		i += 1024;
	}
	if ((size - i) > 0) {
		rev = copy_from_user(buffer+i, buff, size - i);
		if(rev < 0) return rev;
		sys_write_console(buff, size-i);
	}	

	return size;
}

int sys_gettime() {
	return zeos_ticks;
}
