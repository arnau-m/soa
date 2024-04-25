#include <libc.h>

char buff[24];

int pid;

int __attribute__((__section__(".text.main")))
main(void)
{
  /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
  /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  char *buff = "\0\0\0\n";
  char *shd_buff = "\0\0\0\n";
  /*char pacman[] = 
    "  _______ _______ _______        ___ ___ _______ ______  \n"
    " |   _   |   _   |   _   |______|   Y   |   _   |   _  \ \n"
    " |.  1   |.  1   |.  1___|______|.      |.  1   |.  |   |\n"
    " |.  ____|.  _   |.  |___       |. \\_/  |.  _   |.  |   |\n"
    " |:  |   |:  |   |:  1   |      |:  |   |:  |   |:  |   |\n"
    " |::.|   |::.|:. |::.. . |      |::.|:. |::.|:. |::.|   |\n"
    " `---'   `--- ---`-------'      `--- ---`--- ---`--- ---'\n";

  buff = "hello world";
  write(1, "\n", strlen("\n"));
  write(1, pacman, strlen(pacman));
  */

  int pid = fork();

  //Proces pare
  if (pid > 0)  
  {
  }

  //Proces fill
  else if (pid == 0)
  {
  }

  //Error en el fork
  else
  {
    write(1, "ERROR en el fork()\n", strlen("ERROR en el fork()\n"));
    exit();
  }

  while (1)
  {
    if (pid > 0){
      
    }
    if (pid == 0){

    }
  }
}
