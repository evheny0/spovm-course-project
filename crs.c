#include <ncurses.h>
#include <stdlib.h>
#include <cdk/cdk.h>

int main(int argc, char const *argv[])
{
    WINDOW *mywindow = initscr();
    noecho();
    nodelay(mywindow, TRUE);    // no delay on input
    keypad(stdscr, TRUE);       // allow special keys
    int max_y, max_x;
    char lett;
    start_color();
    initCDKColor();
    wbkgd(mywindow, COLOR_PAIR(7));

    getmaxyx(stdscr, max_y, max_x);

    WINDOW *sub_window = newwin(20, max_x - 3, 1, 1);

    box(sub_window, ACS_VLINE, ACS_HLINE);
    CDKSCREEN *screen = initCDKScreen(sub_window);
    char *dow[] = {"LOL", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    CDKSCROLL *scroll = newCDKScroll(screen, 1, 1, RIGHT, 20, max_x - 3, "<C><#H>TEST", dow, 7, NONUMBERS, A_REVERSE, TRUE, FALSE);
    setCDKScrollBackgroundColor(scroll, "</30>");


    refreshCDKScreen(screen);

    // move(10, 10);
    // printw("TEST!!!");
    // printw("TEST!!!");
    // printw("TEST!!!");
    // printw("TEST!!!");
    // printw("TEST!!!");
    // printw("  %d %d\n", max_x, max_y);
    refresh();
    int j = 0;
    for(unsigned i = 0; true; ++i) {
        // lett = mvgetch(20, 20 + i);
        lett = getch();
        if (lett != ERR) {
            move(30, 30);
            printw(" CHAR: %c  ", lett);
            refresh();
        }
        if (lett == 's') {
            setCDKScrollPosition(scroll, ++j);
            refreshCDKScreen(screen);
        } else if (lett == 'w') {
            setCDKScrollPosition(scroll, --j);
            refreshCDKScreen(screen);
            setCDKScrollItems(scroll, dow, 7, NONUMBERS);
            dow[0] = "asdasdasdasdadsdadasd";
        }
        if (lett == 'c') {
            break;
        }
    }
    endwin();
    return 0;
}
