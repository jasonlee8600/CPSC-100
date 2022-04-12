#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "hypercomplex.h"


// Construct a hypercomplex number a+b*u from the given coordinates.
// Argument u2 gives the value of u*u.
// The effect of operations on numbers with different values of u2
// is undefined.
struct h hPack(double a, double b, double u2) {
    struct h answer;
    answer.a = a;
    answer.b = b;
    answer.u2 = u2;

    return answer;
}

// Return the coordinates of a hypercomplex number
// through the given pointers.
void hUnpack(struct h z, double *a, double *b) {
    *a = z.a;
    *b = z.b;
}

// Return z1 + z2.
struct h hAdd(struct h z1, struct h z2) {
    struct h answer;

    answer.a = z1.a + z2.a;
    answer.b = z1.b + z2.b;
    answer.u2 = z1.u2;

    return answer;
}

// Return z1 - z2.
struct h hSubtract(struct h z1, struct h z2) {
    struct h answer;

    answer.a = z1.a - z2.a;
    answer.b = z1.b - z2.b;
    answer.u2 = z1.u2;

    return answer;
}

// Return z1 * z2.
struct h hMultiply(struct h z1, struct h z2) {
    struct h answer;

    answer.a = (z1.a * z2.a) + (z1.b * z2.b * z1.u2);
    answer.b = (z1.a * z2.b) + (z1.b * z2.a);
    answer.u2 = z1.u2;

    return answer;
}

// Given z == a + b*u, return its conjugate a - b*u.
struct h hConjugate(struct h z) {
    struct h answer;

    answer.a = z.a;
    answer.b = z.b * (-1);
    answer.u2 = z.u2;

    return answer;
}

// Return q such that q*z2 == z1.
// If z2 is a zero divisor, behavior is undefined.
struct h hDivide(struct h z1, struct h z2) {

    struct h answer;

    answer.a = ((z1.a * z2.a) - (z1.b * z2.b * z1.u2)) / ((z2.a * z2.a) - (z2.b * z2.b * z2.u2));
    answer.b = ((z2.a * z1.b) - (z1.a * z2.b)) / ((z2.a * z2.a) - (z2.b * z2.b * z2.u2));
    answer.u2 = z1.u2;

    return answer;
}