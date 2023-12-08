#ifndef UTILS_H_
#define UTILS_H_

#define COMMAND_BUFFER_SIZE 2

struct Cursor {
    int x;
    int y;
};

struct Cursor create_new_cursor();
char* allocate_command_buffer();

#endif // UTILS_H_
