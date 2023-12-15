#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>

struct Cursor {
    int x;
    int y;
};

struct CommandNode {
    char command_ch;
    struct CommandNode *next_node;
};

struct Cursor create_new_cursor();

struct CommandNode* allocate_command_head();
void list_push_command_ch(char command_ch, struct CommandNode** head_node);
size_t command_list_size(struct CommandNode* head_node);
char* create_command_string(struct CommandNode* head_node);
void print(struct CommandNode* head_node);
void free_command_list(struct CommandNode** head_node);
void pop_command(struct CommandNode** head_node);

#endif // UTILS_H_
