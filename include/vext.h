#ifndef VEXT_H_
#define VEXT_H_

#include "./utils.h"

#define ESC_KEY 27
#define COMMAND_BUFFER_SIZE 2

enum State {
    NAVIGATE,
    EDIT
};

void vext_core();
void vext_dispatch_command(char *command_buffer);
void vext_navigate(char ch, struct Cursor *cursor, char *command_buffer);
void vext_edit(char ch);

#endif // VEXT_H_
