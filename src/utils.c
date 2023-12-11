#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"
#include <stdlib.h>

struct Cursor create_new_cursor() {
    struct Cursor cursor;
    cursor.x = 0;
    cursor.y = 0;

    return cursor;
}

struct CommandNode* allocate_command_head() {
    struct CommandNode *head_node = NULL;
    return head_node;
}

void list_push_command_ch(char command_ch, struct CommandNode **head_node) {
    struct CommandNode *command_ch_node = malloc(sizeof(struct CommandNode));
    command_ch_node->command_ch = command_ch;
    command_ch_node->next_node = NULL;

    if (*head_node == NULL) {
        *head_node = command_ch_node;
    } else {
        struct CommandNode *node_buffer = *head_node;
        while (node_buffer->next_node != NULL) {
            node_buffer = node_buffer->next_node;
        }

        node_buffer->next_node = command_ch_node;
    }
}

size_t command_list_size(struct CommandNode* head_node) {
    size_t list_size = 0;
    while (head_node != NULL) {
        list_size++;
        head_node = head_node->next_node;
    }

    return list_size;
}

char* create_command_string(struct CommandNode* head_node) {
    size_t list_size = command_list_size(head_node);
    char* command_string = malloc(sizeof(char) * (list_size + 1));
    int index_c = 0;

    while (head_node != NULL) {
        command_string[index_c] = head_node->command_ch;
        index_c++;
        head_node = head_node->next_node;
    }

    command_string[index_c] = '\0';

    return command_string;
}

void print(struct CommandNode* head_node) {
    while (head_node != NULL) {
        addch(head_node->command_ch);
        addch('\n');
        head_node = head_node->next_node;
    }
}

void free_command_list(struct CommandNode** head_node) {
    struct CommandNode* tail_node = *head_node;
    struct CommandNode* node_buffer;

    while (tail_node != NULL) {
        node_buffer = tail_node->next_node;
        free(tail_node);
        tail_node = node_buffer;
    }

    *head_node = NULL;
}
