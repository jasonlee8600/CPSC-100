#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "hypercomplex.h"

// RPN hypercomplex calculator
//
// Takes u*u as argv[1], then processes commands.
//
// Commands are:
//
// "="[number] [number] - push a + y u onto stack
// "+" add second-to-top with top
// "-" subtract top from second-to-top
// "*" multiply second-to-top with top
// "/" divide second-to-top by top
// "|" compute conjugate of top of stack
// "p" print top of stack (without removing it)
// "d" drop top of stack
// "D" duplicate top of stack
// "O" duplicate second-to-top
// "S" swap top and second-to-top
// "#" comment - ignore all characters to end of line
//
// Each operation consumes its arguments from the stack
// and pushes result to the stack.
//
// White space characters are ignored except to end comments
// and numbers.
//
// Elements of stack not supplied by the user act like default
// value zero.

// Basic dynamic array stack.
struct stack {
    size_t top;     // first unused slot in data
    size_t size;    // number of slots in data
    struct h zero;  // default value
    struct h *data; // elements
};

#define STACK_INITIAL_SIZE (4)
#define STACK_SIZE_MULTIPLIER (2)

static struct stack *
stackCreate(struct h zero)
{
    struct stack *s = malloc(sizeof(struct stack));
    assert(s);

    s->top = 0;
    s->size = STACK_INITIAL_SIZE;
    s->zero = zero;
    s->data = calloc(s->size, sizeof(struct h));
    assert(s->data);

    return s;
}

static void
stackDestroy(struct stack *s)
{
    free(s->data);
    free(s);
}

// Return top of stack, or default if stack empty.
static struct h
stackPeek(struct stack *s)
{
    if(s->top > 0) {
        return s->data[s->top-1];
    } else {
        return s->zero;
    }
}

// Remove and return top of stack.
static struct h
stackPop(struct stack *s)
{
    struct h z = stackPeek(s);

    if(s->top > 0) {
        s->top--;
    }

    return z;
}

// Push a new element onto stack
static void
stackPush(struct stack *s, struct h z)
{
    if(s->top >= s->size) {
        s->size *= STACK_SIZE_MULTIPLIER;
        s->data = realloc(s->data, s->size * sizeof(struct h));
    }

    s->data[s->top++] = z;
}

int
main(int argc, char **argv)
{
    double u2;

    if(argc != 2 || sscanf(argv[1], "%lf", &u2) != 1) {
        fprintf(stderr, "Usage: %s u*u\n", argv[0]);
        return 1;
    }

    struct stack *s = stackCreate(hPack(0,0,u2));

    int c;

    while((c = getchar()) != EOF) {
        switch(c) {

            case '=':
                {
                    double a;
                    double b;

                    if(scanf("%lf %lf", &a, &b) != 2) {
                        fprintf(stderr, "Couldn't parse number\n");
                    } else {
                        stackPush(s, hPack(a, b, u2));
                    }
                }

                break;

            case '+':
            case '-':
            case '*':
            case '/':
                {
                    // Factor out pops,
                    // Be careful about order.
                    struct h z2 = stackPop(s);
                    struct h z1 = stackPop(s);
                    struct h result;

                    switch(c) {
                        case '+':
                            result = hAdd(z1, z2);
                            break;
                        case '-':
                            result = hSubtract(z1,z2);
                            break;
                        case '*':
                            result = hMultiply(z1,z2);
                            break;
                        case '/':
                            result = hDivide(z1,z2);
                            break;
                        default:
                            // can't happen
                            assert(0);
                            break;
                    }

                    stackPush(s, result);
                }

                break;

            case '|':
                stackPush(s, hConjugate(stackPop(s)));
                break;

            case 'p':
                {
                    double a;
                    double b;

                    hUnpack(stackPeek(s), &a, &b);
                    printf("%g %g\n", a, b);
                }

                break;

            case 'd':
                stackPop(s);
                break;

            case 'D':
                stackPush(s, stackPeek(s));
                break;

            case 'O':
                {
                    struct h top = stackPop(s);
                    struct h snd = stackPeek(s);
                    stackPush(s, top);
                    stackPush(s, snd);
                }

                break;

            case 'S':
                {
                    struct h top = stackPop(s);
                    struct h snd = stackPop(s);
                    stackPush(s, top);
                    stackPush(s, snd);
                }
                break;

            case '#':
                // eat everything to newline or EOF
                while((c = getchar()) != '\n' && c != EOF);
                break;

            case ' ':
            case '\n':
            case '\t':
                break;

            default:
                fprintf(stderr, "Unrecognized command character '%c'\n", c);
                break;
        }
    }

    stackDestroy(s);

    return 0;
}
