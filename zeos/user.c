#include <libc.h>
#include <interrupt.h>

char buff[24];

int pid;

int __attribute__((__section__(".text.main")))
main(void)
{
    int ret_read = 0;
    char buff[5];

    // Netejar la pantalla

    for (int i = 0; i < 100; i++)
    {
        write(1, "                                                                           \n", strlen("                                                                           \n"));
    }

    int x = 39;
    int y = 13;

    gotoxy(x, y);
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
                        y--;
                    }
                    else if (buff[i] == 's')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        y++;
                    }
                    else if (buff[i] == 'd')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        x++;
                    }
                    else if (buff[i] == 'a')
                    {
                        gotoxy(x,y);
                        write(1, " ", strlen(" "));
                        x--;
                    }
                    gotoxy(x, y);
                    write(1, "c", strlen("c"));
                }
            }
        }
    }
    while (1)
    {
    }
}
