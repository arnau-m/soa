#include <libc.h>
#include <interrupt.h>

char buff[24];

int pid;
int frames = 1;
int frames2 = 0;

int calc_fps()
{
    int time = gettime();
    if (time < 10)
        return 0;
    return frames / (time / 18);
}

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

    while (pid)
    {

        frames++;
        print_map();
        if (frames2 != frames)
        {
            frames2 = frames;
            gotoxy(0, 15);
            write(1, "FPS: ", strlen("FPS: "));
            int fps = calc_fps();
            itoa(fps, buff);
            write(1, buff, strlen(buff));
            write(1, " ", 1);
        }
    }

    while (1)
    {
    }
}
