#include <stdlib.h>

#include "../include/ncurses/ncurses.h"
#include "../include/vext.h"
#include "../include/utils.h"

int main(int argc, char* argv[]) {
    if (argv[1] != NULL) {
        char* file_path = argv[1];

        initscr();
        noecho();
        keypad(stdscr, TRUE);
        set_escdelay(1);
        cbreak();

        vext_core(file_path);

        endwin();
        return 0;
    }

    printf(FILE_PATH_NOT_GIVEN);
    return 1;
}
