#include <libc.h>
#include <interrupt.h>

char buff[24];

int pid;

int __attribute__((__section__(".text.main")))
main(void)
{

    /*
    int ret_read = 0;
    char buff[5];

    // Netejar la pantalla

    for (int i = 0; i < 100; i++)
    {
        write(1, "                                                                           \n", strlen("                                                                           \n"));
    }

    int x = 39;
    int y = 13;

    set_color(2,0);
    gotoxy(x, y);
    shmem();
    write(1, "c", strlen("c"));

    int pid = fork();

    if (pid == 0)
    {
        while (1)
        {
            ret_read = read(&buff, 2);
            if (ret_read != 0)
            {
                for (int i = 0; i < ret_read; i++)
                {

                    if (buff[i] == 'w')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        set_color(2,0);
                        y--;
                    }
                    else if (buff[i] == 's')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        set_color(3,0);
                        y++;
                    }
                    else if (buff[i] == 'd')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        set_color(4,0);
                        x++;
                    }
                    else if (buff[i] == 'a')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        set_color(6,0);
                        x--;
                    }
                    gotoxy(x, y);
                    write(1, "c", strlen("c"));
                }
            }
        }
    }
    */
    set_color(2,0);


    int a = shmat(1, 4096);
    char buf[8];
    itoa(a,buff);

    write(1, buff, strlen(buff));
    
    while (1)
    {
    }
}
