#include <stdio.h>
#include <curses.h>
#include <sys/select.h>

int kbhit(void);

int main()
{
    int i = 0;
    char cad[50];
    initscr();
    while (1)
    {
        mvprintw(3, 4, "%d\n", i);
        refresh();
        if (kbhit())
            mvscanw(10, 5, "%s", cad);
        i++;
    }

    endwin();
}

int kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    if (select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;

    if (FD_ISSET(0, &read_fd))
        return 1;

    return 0;
}
