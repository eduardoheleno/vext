#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 64

#define INVALID_COMMAND "Unknown command"
#define FILE_PATH_NOT_GIVEN "The file path wasn't given\n"

struct Cursor {
    int x;
    int y;
};

struct Line {
    size_t size;
    char* content;
    struct Line* prev_line;
    struct Line* next_line;
};

struct CommandNode {
    char command_ch;
    struct CommandNode* next_node;
};

struct Cursor create_new_cursor();

struct CommandNode* allocate_command_head();
void list_push_command_ch(char command_ch, struct CommandNode** head_node);
size_t command_list_size(struct CommandNode* head_node);
char* create_command_string(struct CommandNode* head_node);
void print(struct CommandNode* head_node);
void free_command_list(struct CommandNode** head_node);
void pop_command(struct CommandNode** head_node);

struct Line* allocate_line(char line_buffer[BUFFER_SIZE]);
void allocate_file_chunks(FILE* file, struct Line** head_line);

#endif // UTILS_H_
