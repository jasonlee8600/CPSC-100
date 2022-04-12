// Functions for doing arithmetic on
// arbitrary-precision decimal numbers.
//
// Each number is represented as an array
// of chars, one per digit,
// with the least significant digit first.
//
// For example, 123 would be stored in a[]
// with a[0] == 3, a[1] == 2, and a[2] == 1.

// Error codes
#define DECIMAL_OK (0)           // returned if operation succeeds
#define DECIMAL_OVERFLOW (1)     // returned if operation would overflow
#define DECIMAL_BAD_DIGIT (2)    // returned for digits outside 0..9

// Used to represent numbers with bad digits in the output.
#define DECIMAL_BAD_OUTPUT ("BAD")

// Base for arithmetic.
// This will not change, but is provided for convenience.
#define DECIMAL_BASE (10)

// Adds addend to augend,
// leaving result in augend.
// Argument precision gives size
// of both augend and addend.
//
// Returns DECIMAL_OK with no errors,
// DECIMAL_OVERFLOW if sum would overflow,
// DECIMAL_BAD_DIGIT if either argument
// includes a digit outside the range 0..9.
//
// If an error occurs, augend should not be changed.
int decimalAdd(size_t precision, char augend[], const char addend[]);

// Subtracts subtrahend from minuend,
// leaving result in minuend.
// Argument precision gives size of both
// minuend and subtrahend.
//
// Returns DECIMAL_OK with no errors,
// DECIMAL_OVERFLOW if result would be negative,
// DECIMAL_BAD_DIGIT if either argument
// includes a digit outside the range 0..9.
//
// If an error occurs, minuend should not be changed.
int decimalSubtract(size_t precision, char minuend[], const char subtrahend[]);

// Print number with given precision to stdout, omitting leading zeros.
// If number contains bad digits, print the string DECIMAL_BAD_OUTPUT.
void decimalPrint(size_t precision, const char number[]);