#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} Stack;

static StatusCode stack_init(Stack *s, size_t initial_capacity) {
    if (initial_capacity == 0) return INVALID_ARGUMENT;
    
    s->data = (char*)malloc(initial_capacity * sizeof(char));
    if (!s->data) return MEMORY_ERROR;
    
    s->size = 0;
    s->capacity = initial_capacity;
    return SUCCESS;
}

static void stack_free(Stack *s) {
    free(s->data);
    s->data = NULL;
    s->size = s->capacity = 0;
}

static StatusCode stack_push(Stack *s, char c) {
    if (s->size >= s->capacity) {
        size_t new_capacity = s->capacity * 2;
        char *new_data = (char*)realloc(s->data, new_capacity);
        if (!new_data) return MEMORY_ERROR;
        
        s->data = new_data;
        s->capacity = new_capacity;
    }
    
    s->data[s->size++] = c;
    return SUCCESS;
}

static StatusCode stack_pop(Stack *s, char *out) {
    if (s->size == 0) return BRACKETS_NOT_BALANCED;
    *out = s->data[--s->size];
    return SUCCESS;
}

static int is_open_bracket(char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

static int is_close_bracket(char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
}

static char get_matching(char open) {
    switch (open) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
        default: return '\0';
    }
}

StatusCode check_brackets(const char *str) {
    if (!str) return INVALID_ARGUMENT;

    int size = 0;
    
    Stack stack;
    StatusCode res = stack_init(&stack, 16);
    if (res != SUCCESS) return res;
    
    for (size_t i = 0; str[i] != '\0'; ++i) {
        char c = str[i];
        
        if (is_open_bracket(c)) {
            res = stack_push(&stack, c);
            if (res != SUCCESS) {
                stack_free(&stack);
                return res;
            }
        } 
        else if (is_close_bracket(c)) {
            char top;
            res = stack_pop(&stack, &top);
            if (res != SUCCESS || get_matching(top) != c) {
                stack_free(&stack);
                return (res == SUCCESS) ? BRACKETS_NOT_BALANCED : res;
            }
        }
    }
    
    size = stack.size;
    stack_free(&stack);
    return (size == 0) ? SUCCESS : BRACKETS_NOT_BALANCED; 
}