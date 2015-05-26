#include "interface.h"  

CDKSCREEN *connections_screen;
CDKSCROLL *connections_scroll;
int position = 0;


void init_window()
{
    int max_y, max_x;
    WINDOW *mywindow = initscr();

    noecho();
    nodelay(mywindow, TRUE);    // no delay on input
    keypad(stdscr, TRUE);       // allow special keys
    initCDKColor();
    wbkgd(mywindow, COLOR_PAIR(30));
    getmaxyx(stdscr, max_y, max_x);
    waddstr(mywindow, "  ** NETWORK MONITOR");

    WINDOW *sub_window = newwin(max_y - 3, max_x, 1, 0);

    box(sub_window, ACS_VLINE, ACS_HLINE);
    connections_screen = initCDKScreen(sub_window);
    char *strs[] = {"empty"};
    connections_scroll = newCDKScroll(connections_screen, 1, 1, RIGHT, max_y - 3, max_x, "<C><#H>Addresses list", strs, 1, NONUMBERS, A_REVERSE, TRUE, FALSE);
    setCDKScrollBackgroundColor(connections_scroll, "</30>");

    refreshCDKScreen(connections_screen);
    refresh();
    refreshCDKScreen(connections_screen);

}

void refresh_screen()
{
    setCDKScrollItems(connections_scroll, connects_list, connections_num, NONUMBERS);
    setCDKScrollPosition(connections_scroll, position);
    refreshCDKScreen(connections_screen);
}

int check_keys()
{
    char key;
    key = getch();
    switch (key) {
    case 'w':
        if (position <= 0) {
            break;
        }
        setCDKScrollPosition(connections_scroll, --position);
        break;
    case 's':
        if (position >= connections_num) {
            break;
        }
        setCDKScrollPosition(connections_scroll, ++position);
        break;
    case 'q':
        endwin();
        return 1;
    default:
        return 0;
    }
    refreshCDKScreen(connections_screen);
    return 0;
}
