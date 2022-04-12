// Infinite streams of characters.
//
// Stream objects can yield character using streamNext,
// or can be used to construct other streams.
//
// When constructing a composite stream, each stream
// should be used only once. This avoids oddities where
// characters from some stream are consumed in multiple
// places, and allows all streams used in the construction
// to be freed using a single call to streamDestroy.

typedef struct stream Stream;

// Return the next character in stream
int streamNext(Stream *stream);

// Return a stream that repeats the contents of s
// over and over.
// For empty string, returns infinite stream of '\0'
// This should copy s in case s is changed or freed.
//
// streamFromString("abc") -> "abcabcabcabcabcabc..."
Stream *streamFromString(const char *s);

// Return contents of even interleaved with contents of odd.
//
// streamInterleave(streamFromString("a"), streamFromString("bc"))
//  -> "abacabacabac..."
Stream *streamInterleave(Stream *even, Stream *odd);

// Return stream where each character c is replaced
// by f(c). Both c and f(c) should be in the range 0..255.
Stream *streamMap(int (*f)(int), Stream *stream);

// Free stream and any streams used to construct stream.
void streamDestroy(Stream *stream);
