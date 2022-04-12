// Split and join operations for strings

// Given an array of pointers to strings,
// terminated by a null pointer, 
// concatenate them separated by character c,
// or no character if c is '\0'.
// Return the result as a malloc'd string.
//
// Examples:
//
// char *a[] = { "ab", "cd", "efg", 0 };
// join(a, ':') returns "ab:cd:efg"
// join(a, '\0') returns "abcdefg"
//
char *join(char **a, int c);

// Reverse of join: given a string s containing
// separators c, construct an array of pointers
// to copies of substrings of s separated by c.
//
// If c is null, a copy of the entire string will be
// returned as the only substring.
//
// Return value may include malloc'd data that can
// be freed with freeSplit if not modified.
//
// Examples:
//
// split("a:b:c", ':') returns {"a", "b", "c", 0}
// split("a::bc", ':') returns {"a", "", "bc", 0}
// split("a::bc", '\0') returns {"a::bc", 0}
//
// (The initializer syntax in the return values
// is for illustration only.)
char **split(const char *s, int c);

// Free all space used by a, assuming a
// was returned by split() and not subsequently
// modified.
void freeSplit(char **a);