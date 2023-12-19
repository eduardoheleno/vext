#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/ncurses/ncurses.h"

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

void list_push_command_ch(char command_ch, struct CommandNode** head_node) {
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

void pop_command(struct CommandNode** head_node) {
    struct CommandNode* current_tail_node = *head_node;
    if (current_tail_node == NULL) return;

    struct CommandNode* next_tail_node = (*head_node)->next_node;
    if (next_tail_node == NULL) {
        free(current_tail_node);
        *head_node = NULL;
        return;
    }

    while (next_tail_node->next_node != NULL) {
        current_tail_node = next_tail_node;
        next_tail_node = next_tail_node->next_node;
    }

    free(next_tail_node);
    current_tail_node->next_node = NULL;
}

struct Line* allocate_line(char line_buffer[BUFFER_SIZE]) {
    char* owned_buffer = malloc(BUFFER_SIZE);
    strcpy(owned_buffer, line_buffer);

    struct Line* line = malloc(sizeof(struct Line));
    line->content = owned_buffer;
    line->size = strlen(line_buffer);
    line->prev_line = NULL;
    line->next_line = NULL;

    return line;
}

void allocate_file_chunks(FILE* file, struct Line** head_line) {
    char line_buffer[BUFFER_SIZE];
    struct Line* current_line = NULL;
    struct Line* prev_line = NULL;

    while (fgets(line_buffer, BUFFER_SIZE, file) != NULL) {
        struct Line* line = allocate_line(line_buffer);
        if (*head_line == NULL) {
            *head_line = line;
            prev_line = *head_line;
        } else {
            current_line = line;

            prev_line->next_line = current_line;
            current_line->prev_line = prev_line;

            prev_line = current_line;
            current_line = current_line->next_line;
        }
    }
}
