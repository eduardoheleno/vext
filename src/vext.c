#include "../include/vext.h"
#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"

#include <stdlib.h>
#include <string.h>

#define QUIT_COMMAND ":q"

struct Line* head_line                      = NULL;
struct Line* line_tracker                   = NULL;

struct CommandNode* head_node               = NULL;
enum State state                            = NAVIGATE;
static int exit_vext                        = 0;

static WINDOW* command_window               = NULL;
static struct Cursor* command_window_cursor = NULL;

// TODO: fix -> if a line is bigger than the buffer size, the program interprets that the next line buffer will be a new line.

void vext_dispatch_command() {
    char* command_string = create_command_string(head_node);

    if (strcmp(command_string, QUIT_COMMAND) == 0) {
        exit_vext = 1;
        goto free_command;
    }

    command_window_clear();
    wprintw(command_window, INVALID_COMMAND);
    wrefresh(command_window);

free_command:
    free(command_string);
    free_command_list(&head_node);
    state = NAVIGATE;
    return;
}

void command_window_insert_ch(char ch) {
    command_window_clear();

    char* command_string = create_command_string(head_node);
    size_t command_size = command_list_size(head_node);
    wprintw(command_window, "%s", command_string);
    wrefresh(command_window);

    command_window_cursor->x = command_size;
}

void command_window_clear() {
    command_window_cursor->x = 0;
    wclear(command_window);
    wrefresh(command_window);
}

void command_window_remove_ch() {
    command_window_cursor->x--;

    pop_command(&head_node);
    mvwdelch(command_window, command_window_cursor->y, command_window_cursor->x);
    wrefresh(command_window);

    if (command_window_cursor->x == 0) {
        state = NAVIGATE;
        return;
    }
}

void vext_navigate(char ch, struct Cursor* cursor) {
    switch(ch) {
        case 'j':
            if (line_tracker->next_line == NULL) return;
            line_tracker = line_tracker->next_line;

            if (line_tracker->size == 1) {
                cursor->y = 0;
            } else if (cursor->max_y > line_tracker->size - 2) {
                cursor->y = line_tracker->size - 2;
            } else {
                cursor->y = cursor->max_y;
            }

            cursor->x++;
            break;
        case 'k':
            if (line_tracker->prev_line == NULL) return;
            line_tracker = line_tracker->prev_line;

            if (line_tracker->size == 1) {
                cursor->y = 0;
            } else if (cursor->max_y > line_tracker->size - 2) {
                cursor->y = line_tracker->size - 2;
            } else {
                cursor->y = cursor->max_y;
            }

            cursor->x--;
            break;
        case 'h':
            if (cursor->y == 0) return;
            cursor->y--;
            cursor->max_y = cursor->y;
            break;
        case 'l':
            if (cursor->y == line_tracker->size - 2) return;
            cursor->y++;
            cursor->max_y = cursor->y;
            break;
        case '0':
            cursor->y = 0;
            cursor->max_y = 0;
            break;
        case DOLLAR_SIGN_KEY:
            cursor->y = line_tracker->size - 2;
            cursor->max_y = cursor->y;
            break;
        default:
            state = COMMAND_EDIT;
            vext_command_edit(ch);
            break;
    }
}

void vext_command_edit(char ch) {
    switch (ch) {
        case '\n':
            vext_dispatch_command();
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

void vext_render_lines(struct Line* head_line) {
    struct Line* current_line = head_line;
    while (current_line != NULL) {
        printw("%s", current_line->content);
        refresh();
        current_line = current_line->next_line;
    }
}

void vext_core(char* file_path) {
    FILE* file = fopen(file_path, "r+");
    if (file == NULL) {
        perror("File error");
        exit(1);
    }

    allocate_file_chunks(file, &head_line, &line_tracker);
    vext_render_lines(head_line);

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
