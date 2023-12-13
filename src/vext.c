#include "../include/vext.h"
#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// TODO: remove the use of this function in different parts of the code.
void handle_command_subwindow(bool free_command_subwindow, char ch) {
    static WINDOW* command_window = NULL;
    static struct Cursor* command_window_cursor = NULL;

    if (command_window == NULL && command_window_cursor == NULL) {
        int main_window_max_y = getmaxy(stdscr) - 1;
        command_window_cursor = malloc(sizeof(struct Cursor));

        command_window = subwin(stdscr, 0, 0, main_window_max_y, 0);
        *command_window_cursor = create_new_cursor();
    }

    if (free_command_subwindow) {
        wclear(command_window);
        delwin(command_window);
        command_window = NULL;

        free(command_window_cursor);
        command_window_cursor = NULL;

        return;
    }

    mvwaddch(command_window, command_window_cursor->y, command_window_cursor->x, ch);
    wrefresh(command_window);

    command_window_cursor->x++;
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
            handle_command_subwindow(true, ' ');
            break;
        default:
            list_push_command_ch(ch, head_node);
            handle_command_subwindow(false, ch);
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
        handle_command_subwindow(true, ' ');
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
