#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define STACK_SIZE 256

typedef struct {
    char element;
    int line;
    int index;
} StackItem;

typedef struct {
    StackItem items[STACK_SIZE];
    int top;
} Stack;

void stack_init(Stack* s) {
    s->top = -1;
}

bool stack_is_empty(Stack* s) {
    return s->top == -1;
}

void stack_push(Stack* s, char el, int line, int idx) {
    if (s->top >= STACK_SIZE - 1) {
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
    s->top++;
    s->items[s->top] = (StackItem){el, line, idx};
}

StackItem stack_pop(Stack* s) {
    if (stack_is_empty(s)) {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    return s->items[s->top--];
}

bool is_opening(char c) {
    return c == '(' || c == '[' || c == '{' || c == '"' || c == '\'';
}

bool is_closing(char c) {
    return c == ')' || c == ']' || c == '}' || c == '"' || c == '\'';
}

bool matches(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}') ||
           (opening == '"' && closing == '"') ||
           (opening == '\'' &&
            closing == '\'');  // maybe I should use their ascii value ?
}

bool in_comment(char c, char prev_c, bool* single_line, bool* multi_line) {
    if (*single_line && c == '\n') {
        *single_line = false;
    } else if (*multi_line && prev_c == '*' && c == '/') {
        *multi_line = false;
    } else if (!*single_line && !*multi_line) {
        if (prev_c == '/' && c == '/') {
            *single_line = true;
        } else if (prev_c == '/' && c == '*') {
            *multi_line = true;
        }
    }
    return *single_line || *multi_line;
}

void process_char(Stack* stack, char c, char prev_c, int line, int index,
                  bool* in_string, bool* sg_line_comment, bool* ml_line_comment,
                  bool empty_line) {

    if (*in_string) {
        if ((c == '"' || c == '\'') && prev_c != '\\') {
            *in_string = false;
            stack_pop(stack);
        }
        return;
    }

    if (in_comment(c, prev_c, sg_line_comment, ml_line_comment)) {
        return;
    }

    if ((c == '"' || c == '\'') && prev_c != '\\') {
        *in_string = true;
        stack_push(stack, c, line, index);
        return;
    }

    if (is_opening(c)) {
        stack_push(stack, c, line, index);
    } else if (is_closing(c)) {
        if (stack_is_empty(stack)) {
            fprintf(stderr, "Error: Unexpected '%c' on line %d:%d\n", c, line,
                    index);
            return;
        }
        StackItem top = stack_pop(stack);
        if (!matches(top.element, c)) {
            fprintf(stderr,
                    "Error: Mismatched '%c' on line %d:%d. Expected matching "
                    "'%c' from line %d:%d\n",
                    c, line, index, top.element, top.line, top.index);
        }
    }
}

void check_file(FILE* file) {
    char buffer[BUFFER_SIZE];
    int line_number = 1;
    char prev_c = 0;
    Stack stack;
    stack_init(&stack);
    bool sg_line_comment = false;  // Inside a single line comment ?
    bool ml_line_comment = false;  // Inside a "/*" comment ?
    bool in_string = false;
    bool empty_line = false;

    while (fgets(buffer, BUFFER_SIZE, file)) {
        empty_line = (buffer[0] == '\n');
        for (int i = 0; buffer[i] != '\0'; i++) {
            process_char(&stack, buffer[i], prev_c, line_number, i + 1,
                         &in_string, &sg_line_comment, &ml_line_comment,
                         empty_line);
            prev_c = buffer[i];
        }
        line_number++;
    }

    while (!stack_is_empty(&stack)) {
        StackItem unmatched = stack_pop(&stack);
        fprintf(stderr, "Unmatched '%c' on line %d:%d\n", unmatched.element,
                unmatched.line, unmatched.index);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Can't open %s\n", argv[1]);
        return 1;
    }

    check_file(file);
    fclose(file);
    return 0;
}
