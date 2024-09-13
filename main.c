#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define STACK_SIZE 256

enum SyntaxElement {
    OP_PARENTH = 1,
    OP_BRACKET,
    OP_BRACE,
    CL_PARENTH,
    CL_BRACKET,
    CL_BRACE,
};

int SYNTAX_STACK[STACK_SIZE];
int CURSOR_IDX = 0;
int S_SIZE = 0;

int push_stack(int value) {
    S_SIZE++;
    return SYNTAX_STACK[CURSOR_IDX++] = value;
}

int pop_stack(void) {
    if (S_SIZE > 0) {
        S_SIZE--;
        return SYNTAX_STACK[--CURSOR_IDX];
    }
    if (S_SIZE < 0)
        S_SIZE = 0;
    return -1;
}

int peek_stack(int pos) {
    if (S_SIZE > 0 && pos < S_SIZE) {
        return SYNTAX_STACK[(CURSOR_IDX - 1 - pos)];
    }
    return -1;
}

int peek_stack_until(int value) {
    int ret, i;
    i = 0;
    while ((ret = peek_stack(i) != -1)) {
        if (ret == value)
            return value;
        i++;
    }
    return -1;
}

void checkmatch() {}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: detab file1\n");
        exit(1);
    }

    char* pathname = argv[1];
    FILE* file = fopen(pathname, "r");
    if (file == NULL) {
        fprintf(stderr, "Can't open %s\n", pathname);
        return 0;
    }

    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    while ((fgets(buffer, BUFFER_SIZE, file))) {
        int c, i;
        i = 0;
        while ((c = buffer[i]) != EOF && c != '\0') {
            switch (c) {
                case '(':
                    push_stack(OP_PARENTH);
                    break;
                case '[':
                    push_stack(OP_BRACKET);
                    break;
                case '{':
                    push_stack(OP_BRACE);
                    break;
                case ')':
                    push_stack(CL_PARENTH);
                    break;
                case ']':
                    push_stack(CL_BRACKET);
                    break;
                case '}':
                    push_stack(CL_BRACE);
                    break;
            }
            i++;
        }
    }

    fclose(file);
    free(buffer);
    return 0;
}
