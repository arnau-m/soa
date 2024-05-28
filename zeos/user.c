#include <libc.h>
#include <interrupt.h>

char buff[24];

int pid;

void print_map()
{
    gotoxy(0, 0);
    set_color(2, 0);
    char *map = "###################################\n#..............##.................#\n#..###..####..##..####..####..###.#\n#..###..####..##..####..####..###.#\n#.............##................. #\n###..##..##..####..##..####..##..##\n###..##..##..####..##..####..##..##\n#........##....P.........##.......#\n#####..####..##..####..##..####..##\n#####..####..##..####..##..####..##\n#.............##................. #\n#..###..####..##..####..####..###.#\n#..###..####..##..####..####..###.#\n#..............##.................#\n###################################\n";
    write(1, map, strlen(map));
}

int __attribute__((__section__(".text.main")))
main(void)
{
    // Netejar la pantalla

    for (int i = 0; i < 100; i++)
    {
        write(1, "                                                                           \n", strlen("                                                                           \n"));
    }

    gotoxy(0, 0);
    set_color(2, 0);

    int pid = fork();

    if (pid == 0)
    {
        int frame = 0;
        int newTime = 0;
        int time = 0;
        while (1)
        {

            print_map();

            frame++;
            newTime = gettime();
            if (newTime % 18 == 0 && newTime != time)
            {
                gotoxy(0, 15);
                write(1, "FPS: ", strlen("FPS: "));
                int diff = newTime - time;
                itoa(frame, buff);
                write(1, buff, strlen(buff));
                write(1, " ", strlen(" "));
                frame = 0;
                time = newTime;
            }
        }
    }

    while (1)
    {
    }
}
