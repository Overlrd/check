#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define STACK_SIZE 256

typedef struct {
    char element;
    int line;
    int index;
} StackItem;

StackItem SYNTAX_STACK[STACK_SIZE];
int CURSOR_IDX = 0;
int S_SIZE = 0;

void push_stack(char el, int line, int idx) {
    if (S_SIZE > STACK_SIZE) {
        fprintf(stderr, "Overflow: S_SIZE > STACK_SIZE\n");
        exit(1);
    }
    SYNTAX_STACK[CURSOR_IDX++] = (StackItem){el, line, idx};
    S_SIZE++;
}

StackItem pop_stack(void) {
    if (S_SIZE <= 0) {
        fprintf(stderr, "Underflow: S_SIZE <= 0\n");
        exit(1);
    }
    S_SIZE--;
    return SYNTAX_STACK[--CURSOR_IDX];
}

int matching_pair(char op, char cl) {
    return (op == '(' && cl == ')') || (op == '[' && cl == ']') ||
           (op == '{' && cl == '}');
}

void checkmatch(FILE* file) {
    char buffer[BUFFER_SIZE];
    int i;
    char c;
    int idx;
    int line_number = 1;

    while (fgets(buffer, BUFFER_SIZE, file)) {
        i = 0;
        idx = 0;
        while (buffer[i] != '\0') {
            c = buffer[i];
            idx++;

            switch (c) {
                case '(':
                case '[':
                case '{':
                    push_stack(c, line_number, idx);
                    break;
                case ')':
                case ']':
                case '}':
                    if (S_SIZE == 0) {
                        fprintf(stderr,
                                "Error: Unexpected '%c' on line %d:%d\n", c,
                                line_number, idx);
                        return;
                    }
                    StackItem top = pop_stack();
                    if (!matching_pair(top.element, c)) {
                        fprintf(stderr,
                                "Error: Unexpected '%c' on line:%d:%d. "
                                "(Expected "
                                "matching '%c' from line:%d:%d))\n",
                                c, line_number, idx, top.element, top.line,
                                top.index);
                        return;
                    }
                    break;
                default:
                    break;
            }

            i++;
        }

        line_number++;
    }
    // Check for any remaining unmatched opening elements
    while (S_SIZE > 0) {
        StackItem unmatched = pop_stack();
        fprintf(stderr, "Unmatched '%c' on line:%d:%d\n", unmatched.element,
                unmatched.line, unmatched.index);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s file\n", argv[0]);
        exit(1);
    }

    char* pathname = argv[1];
    FILE* file = fopen(pathname, "r");
    if (file == NULL) {
        fprintf(stderr, "Can't open %s\n", pathname);
        return 1;
    }

    checkmatch(file);

    fclose(file);
    return 0;
}
