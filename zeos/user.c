#include <libc.h>
#include <interrupt.h>

char buff[24];

int pid;

int __attribute__((__section__(".text.main")))
main(void)
{
    int ret_read = 0;
    char buff[5];

    for (int i = 0; i < 100; i++)
    {
        write(1, "                                                                           \n", strlen("                                                                           \n"));
    }

    while (1)
    {
        ret_read = read(&buff, 2);
        if (ret_read != 0)
        {
            write(1, "\nHem llegit!", strlen("\nHem llegit!"));
            write(1, "\n", strlen("\n"));
            write(1, buff, strlen(buff));
        }
    }
}
