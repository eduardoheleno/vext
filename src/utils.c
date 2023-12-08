#include "../include/utils.h"
#include <stdlib.h>

struct Cursor create_new_cursor() {
    struct Cursor cursor;
    cursor.x = 0;
    cursor.y = 0;

    return cursor;
}

char* allocate_command_buffer() {
    char *command_buffer = malloc(sizeof(char) * COMMAND_BUFFER_SIZE);
    command_buffer[0] = '\0';
    command_buffer[1] = '\0';

    return command_buffer;
}
