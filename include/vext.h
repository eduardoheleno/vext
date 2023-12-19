#ifndef VEXT_H_
#define VEXT_H_

#include "./utils.h"
#include <stdbool.h>

#define ESC_KEY 27
#define BACKSPACE_KEY 127

enum State {
    NAVIGATE,
    DEFAULT_EDIT,
    COMMAND_EDIT
};

void vext_core();
void vext_default_edit(char ch, struct Cursor* cursor);
void vext_command_edit(char ch);
void vext_navigate(char ch, struct Cursor* cursor);
void vext_dispatch_command();
int detect_state_change(char ch);
void command_window_insert_ch(char ch);
void command_window_clear();
void command_window_remove_ch();

#endif // VEXT_H_
