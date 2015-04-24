#include <ncurses.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    WINDOW *mywindow = initscr();
    noecho();
    nodelay(mywindow, TRUE);    // no delay on input
    keypad(stdscr, TRUE);       // allow special keys
    int max_y, max_x;
    char lett;
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    

    getmaxyx(stdscr, max_y, max_x);

    move(10, 10);
    box(mywindow, ACS_VLINE, ACS_HLINE);
    printw("TEST!!!");
    printw("TEST!!!");
    printw("TEST!!!");
    printw("TEST!!!");
    printw("TEST!!!");
    printw("  %d %d\n", max_x, max_y);
    refresh();
    for(unsigned i = 0; true; ++i) {
        // lett = mvgetch(20, 20 + i);
        lett = getch();
        if (lett != ERR) {
            move(30, 30);
            printw(" CHAR: %c  ", lett);
            refresh();
        }
        if (lett == 'c') {
            break;
        }
    }
    endwin();
    return 0;
}
