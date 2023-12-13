#ifndef VEXT_H_
#define VEXT_H_

#include "./utils.h"
#include <stdbool.h>

#define ESC_KEY 27

enum State {
    NAVIGATE,
    EDIT
};

void vext_core();
void vext_edit(char ch, struct Cursor *cursor);
void vext_navigate(char ch, struct Cursor *cursor, struct CommandNode **head_node);
void vext_dispatch_command(struct CommandNode **head_node);
int detect_state_change(char ch, enum State *state);
void handle_command_subwindow(bool free_command_subwindow, char ch);

#endif // VEXT_H_
