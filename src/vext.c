#include "../include/vext.h"
#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"

#include <stdlib.h>
#include <string.h>

#define QUIT_COMMAND ":q"

struct CommandNode* head_node = NULL;
enum State state = NAVIGATE;
static int exit_vext = 0;

static WINDOW* command_window = NULL;
static struct Cursor* command_window_cursor = NULL;

void vext_dispatch_command(struct CommandNode** head_node) {
    char* command_string = create_command_string(*head_node);
    printw("%s", command_string);
    size_t t = command_list_size(*head_node);

    if (strcmp(command_string, QUIT_COMMAND) == 0) {
        exit_vext = 1;
    }

    free(command_string);
    free_command_list(head_node);
}

void command_window_insert_ch(char ch) {
    mvwaddch(command_window, command_window_cursor->y, command_window_cursor->x, ch);
    wrefresh(command_window);

    command_window_cursor->x++;
}

void command_window_clear() {
    command_window_cursor->x = 0;
    wclear(command_window);
    wrefresh(command_window);
}

void command_window_remove_ch() {
    if (command_window_cursor->x == 0) return;

    command_window_cursor->x--;
    pop_command(&head_node);
    mvwdelch(command_window, command_window_cursor->y, command_window_cursor->x);
    wrefresh(command_window);
}

void vext_navigate(char ch, struct Cursor* cursor) {
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
            state = COMMAND_EDIT;
            vext_command_edit(ch);
    }
}

void vext_command_edit(char ch) {
    switch (ch) {
        case '\n':
            vext_dispatch_command(&head_node);
            command_window_clear();
            break;
        case BACKSPACE_KEY:
            command_window_remove_ch();
            break;
        default:
            list_push_command_ch(ch, &head_node);
            command_window_insert_ch(ch);
            break;
    }
}

void vext_default_edit(char ch, struct Cursor *cursor) {
    addch(ch);
    cursor->y++;
}

int detect_state_change(char ch) {
    if ((int)ch == ESC_KEY) {
        command_window_clear();
        state = NAVIGATE;
        return 1;
    }

    if (ch == 'i') {
        state = DEFAULT_EDIT;
        return 1;
    }

    return 0;
}

void vext_core() {
    struct Cursor cursor = create_new_cursor();
    char ch;

    command_window = subwin(stdscr, 0, 0, getmaxy(stdscr) - 1, 0);
    command_window_cursor = malloc(sizeof(struct Cursor));
    *command_window_cursor = create_new_cursor();

    move(cursor.x, cursor.y);

    while(1) {
        ch = getch();

        int has_state_changed = detect_state_change(ch);
        if (has_state_changed == 1) {
            free_command_list(&head_node);
            continue;
        }

        switch(state) {
            case NAVIGATE:
                vext_navigate(ch, &cursor);
                break;
            case DEFAULT_EDIT:
                vext_default_edit(ch, &cursor);
                break;
            case COMMAND_EDIT:
                vext_command_edit(ch);
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

    delwin(command_window);
    free(command_window_cursor);
}
