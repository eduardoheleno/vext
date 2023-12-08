#include "../include/vext.h"
#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"

#include <stdlib.h>

void vext_dispatch_command(char *command_buffer) {

}

void vext_navigate(char ch, struct Cursor *cursor, char *command_buffer) {
    switch(ch) {
        case 'j':
            cursor->x++;
            break;
        case 'k':
            if (cursor->x == 0) return;
            cursor->x--;
            break;
        case 'h':
            if (cursor->y == 0) return;
            cursor->y--;
            break;
        case 'l':
            cursor->y++;
            break;
        default:
            if (command_buffer[0] == '\0') {
                command_buffer[0] = ch;
            } else if (command_buffer[1] == '\0') {
                command_buffer[1] = ch;
            } else {
                vext_dispatch_command(command_buffer);
            }

            break;
    }
}

void vext_edit(char ch) {
    addch(ch);
}

void vext_core() {
    enum State state = NAVIGATE;
    struct Cursor cursor = create_new_cursor();

    move(cursor.x, cursor.y);

    char *command_buffer = allocate_command_buffer();
    char ch;

    while(1) {
        if ((ch = getch()) == ESC_KEY) break;

        switch(state) {
            case NAVIGATE:
                vext_navigate(ch, &cursor, command_buffer);
                break;
            case EDIT:
                vext_edit(ch);
                break;
            default:
                exit(1);
        }

        move(cursor.x, cursor.y);
        refresh();
    }
}
