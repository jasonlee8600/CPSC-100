#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "decimal.h"

// empty test, always succeeds
static int
testEmpty(size_t precision, char **a, int argc, char **argv)
{
    return 0;
}

// parse a string as a number, returning
// DECIMAL_OK if successful,
// DECIMAL_OVERFLOW if string is too big.
//
// Bad digits get turned into bad digits.
static int
parseNumber(size_t precision, char number[], const char *s)
{
    size_t ss = strlen(s);

    if(ss > precision) {
        return DECIMAL_OVERFLOW;
    } 

    for(size_t i = 0; i < ss; i++) {
        // index hackery reverses number
        number[ss-i-1] = s[i] - '0';
    }

    // zero out extra digits
    memset(number + ss, 0, precision - ss);

    return DECIMAL_OK;
}

// returns 1 on error, 0 otherwise
static int
parseArgs(int expected, size_t precision, char **a, int argc, char **argv)
{
    if(argc != expected + 1) {
        fprintf(stderr, "%s: requires %d numeric argument%s\n", 
                argv[0], expected, expected > 1 ? "s" : ""); 
        return 1;
    }
    // else
    for(int i = 0; i < expected; i++) {
        if(parseNumber(precision, a[i], argv[i+1]) != DECIMAL_OK) {
            fprintf(stderr, "%s: couldn't parse number %s\n", argv[0], argv[i+1]);
            return 1;
        }
    }

    return 0;
}

static int
testPrint(size_t precision, char **a, int argc, char **argv)
{
    if(parseArgs(1, precision, a, argc, argv)) {
        return 1;
    } else {
        decimalPrint(precision, a[0]);
        putchar('\n');
        return 0;
    }
}

static int
testOp(size_t precision, char **a, int (*op)(size_t, char a0[], const char a1[]))
{
    int result;

    result = op(precision, a[0], a[1]);

    printf("%d\n", result);
    decimalPrint(precision, a[0]);
    putchar('\n');
    decimalPrint(precision, a[1]);
    putchar('\n');

    return result != DECIMAL_OK;
}

static int
testAdd(size_t precision, char **a, int argc, char **argv)
{
    if(parseArgs(2, precision, a, argc, argv)) {
        return 1;
    } else {
        return testOp(precision, a, decimalAdd);
    }
}

static int
testSubtract(size_t precision, char **a, int argc, char **argv)
{
    if(parseArgs(2, precision, a, argc, argv)) {
        return 1;
    } else {
        return testOp(precision, a, decimalSubtract);
    }
}

// 5 is a primitive element of â„¤_107
#define RNG_MULTIPLIER (5)
#define RNG_MODULUS (107)

// generate mildly pseudorandom inputs based on precision
static void
randomArgs(size_t precision, char **a)
{
    int seed = precision % RNG_MODULUS;

    for(int i = 0; i < 2; i++) {
        for(size_t j = 0; j < precision; j++) {
            // update seed
            seed = (seed * RNG_MULTIPLIER) % RNG_MODULUS;
            a[i][j] = seed % DECIMAL_BASE;
        }
        // shift seed so we don't get two all-0 numbers
        seed++;
    }
}

static int
testAddHuge(size_t precision, char **a, int argc, char **argv)
{
    randomArgs(precision, a);
    return testOp(precision, a, decimalAdd);
}

static int
testSubtractHuge(size_t precision, char **a, int argc, char **argv)
{
    randomArgs(precision, a);
    return testOp(precision, a, decimalSubtract);
}

struct command {
    const char *command;
    int (*test)(size_t precision, char **a, int argc, char **argv);
};

// dispatch table terminated by null command
struct command Commands[] = {
    { "empty", testEmpty },
    { "print", testPrint },
    { "add", testAdd },
    { "sub", testSubtract },
    { "addHuge", testAddHuge },
    { "subHuge", testSubtractHuge },
    { 0, 0 }
};

int
main(int argc, char **argv)
{

    if(argc < 3) {
        fprintf(stderr, "Usage: %s precision command [args]\n", argv[0]);
        return 2;
    }

    // parse precision and allocate space
    size_t precision;
    if(sscanf(argv[1], "%zu", &precision) != 1) {
        fprintf(stderr, "%s: couldn't parse precision from %s\n", argv[0], argv[1]);
        return 3;
    }

    char *a[2];

    a[0] = calloc(precision, sizeof(char));
    a[1] = calloc(precision, sizeof(char));

    // find command
    for(size_t i = 0; Commands[i].command != 0; i++) {
        if(!strcmp(Commands[i].command, argv[2])) {
            // shift argc, argv so command comes first
            if(Commands[i].test(precision, a, argc-2, argv+2)) {
                // normally we would send this to stderr,
                // but this creates nondeterminism in the output
                printf("%s: test %s failed\n", argv[0], argv[2]);
                // fprintf(stderr, "%s: test %s failed\n", argv[0], argv[2]);
                return 1;
            } else {
                // we are only freeing on the good case
                // since other cases will throw errors anyway
                free(a[0]);
                free(a[1]);
                return 0;
            }
        }
    }

    fprintf(stderr, "%s: bad command %s\n", argv[0], argv[2]);

    return 4;
}