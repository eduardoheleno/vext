#ifndef VEXT_H_
#define VEXT_H_

#include "./utils.h"

#define ESC_KEY 27

enum State {
    NAVIGATE,
    EDIT
};

int detect_state_change(char ch, enum State *state);
void vext_dispatch_command(struct CommandNode **head_node);
void vext_navigate(char ch, struct Cursor *cursor, struct CommandNode **head_node);
void vext_edit(char ch, struct Cursor *cursor);
void vext_core();

#endif // VEXT_H_
