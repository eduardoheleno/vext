#include "../include/vext.h"
#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"

#include <stdlib.h>
#include <string.h>

#define QUIT_COMMAND ":q"

static int exit_vext = 0;

void vext_dispatch_command(struct CommandNode** head_node) {
    char* command_string = create_command_string(*head_node);
    size_t t = command_list_size(*head_node);

    if (strcmp(command_string, QUIT_COMMAND) == 0) {
        exit_vext = 1;
    }

    free(command_string);
    free_command_list(head_node);
}

void vext_navigate(char ch, struct Cursor* cursor, struct CommandNode** head_node) {
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
        case '\n':
            vext_dispatch_command(head_node);
            break;
        default:
            list_push_command_ch(ch, head_node);
            break;
    }
}

void vext_edit(char ch, struct Cursor *cursor) {
    addch(ch);
    cursor->y++;
}

int detect_state_change(char ch, enum State *state) {
    if (ch == ESC_KEY) {
        *state = NAVIGATE;
        return 1;
    }

    if (ch == 'i') {
        *state = EDIT;
        return 1;
    }

    return 0;
}

void vext_core() {
    enum State state = NAVIGATE;
    struct Cursor cursor = create_new_cursor();
    struct CommandNode *head_node = allocate_command_head();
    char ch;

    move(cursor.x, cursor.y);

    while(1) {
        ch = getch();

        int has_state_changed = detect_state_change(ch, &state);
        if (has_state_changed == 1) {
            free_command_list(&head_node);
            continue;
        }

        switch(state) {
            case NAVIGATE:
                vext_navigate(ch, &cursor, &head_node);
                break;
            case EDIT:
                vext_edit(ch, &cursor);
                break;
            default:
                exit(1);
        }

        if (exit_vext == 1) {
            break;
        }

        move(cursor.x, cursor.y);
        refresh();
    }
}
