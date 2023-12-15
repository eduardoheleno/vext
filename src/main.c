#include <stdlib.h>
#include "../include/ncurses/ncurses.h"
#include "../include/vext.h"
#include "../include/utils.h"

int main() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(1);
    cbreak();

    vext_core();

    endwin();

    return 0;
}
