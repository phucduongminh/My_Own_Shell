#include "curses.h"

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    refresh();
    getch();
    endwin();
    return 0;
}