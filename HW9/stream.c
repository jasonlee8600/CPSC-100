#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"


struct stream {
    int type;       // 0 = fromString, 1 = interleave, 2 = map
    
    // for fromString
    char *string;
    int length;     
    int position; 

    int (*f)(int);  // for map
    
    Stream *even;
    Stream *odd;    

    int turn;       // for interleave
};


// from main.c for test case input format
// "<string>" -> streamFromString(<string>)
// x<stream1><stream2> -> streamInterleave(<stream1>,<stream2>)
// +<stream> -> streamMap(plus1, <stream>)


// Return the next character in stream
int streamNext(Stream *stream) {
    // to store next character in steam
    int character;

    // for 3 different stream type cases (0, 1, 2)
    int type = stream->type;

    switch(type) {

        // for fromString stream
        case 0:
            // if empty string, returns infinite stream of '\0' which is == 0 as a decimal
            if (stream->length == 0) {
                character = 0;
            }
            
            else {
                // get character in stream string's current position
                character = stream->string[stream->position];
                // update stream's position field modulo strlen(s) so that it wraps around when it reaches the end of string
                stream->position = (stream->position + 1) % stream->length;
            }
            break;

        // for interleave stream
        case 1:
            // even's turn
            if (stream->turn == 0) {
                // recursive call
                character = streamNext(stream->even);
                stream->turn = 1;
            }
            // odd's turn
            else {
                character = streamNext(stream->odd);
                stream->turn = 0;
            }
            break;

        // for map stream
        case 2:
            // declare function pointer and set it to given function from streamMap
            int (*f) (int);
            f = stream->f;
            // apply function to even stream where stream argument that was passed in streamMap is held
            character = f(streamNext(stream->even));
            break;
    }

    return character;
}


// Return a stream that repeats the contents of s over and over.
// For empty string, returns infinite stream of '\0'
// This should copy s in case s is changed or freed.
// streamFromString("abc") -> "abcabcabcabcabcabc..."
Stream *streamFromString(const char *s) 
{
    Stream *answer = malloc(sizeof(Stream));

    // copy s in case s is changed or freed
    const char *copy = s;

    answer->type = 0;

    answer->string = calloc(strlen(copy), sizeof(char));
    // put string copy into stream->string, excluding the '\0' at end of the string
    strncpy(answer->string, copy, strlen(copy));

    answer->length = strlen(s);
    answer->position = 0;
    answer->f = NULL;
    answer->even = NULL;
    answer->odd = NULL;
    answer->turn = 0;

    return answer;
}


// Return contents of even interleaved with contents of odd.
// streamInterleave(streamFromString("a"), streamFromString("bc")) -> "abacabacabac..."
Stream *streamInterleave(Stream *even, Stream *odd)
{
    Stream *answer = malloc(sizeof(Stream));
    
    answer->type = 1;   
    answer->string = NULL;
    answer->length = 0;
    answer->position = 0;
    answer->f = NULL;
    answer->even = even;
    answer->odd = odd;
    answer->turn = 0;

    return answer;
}


// Return stream where each character c is replaced by f(c). 
// Both c and f(c) should be in the range 0..255.
Stream *streamMap(int (*f)(int), Stream *stream)
{
    Stream *answer = malloc(sizeof(Stream));

    answer->type = 2;   
    answer->string = NULL;
    answer->length = 0;
    answer->position = 0;
    answer->f = *f;
    answer->even = stream;
    answer->odd = NULL;
    answer->turn = 0;

    return answer;
}

// Free stream and any streams used to construct stream.
void streamDestroy(Stream *stream)
{
    int type = stream->type;

    switch(type) {
        case 0:
            free(stream->string);
            free(stream);
            break;

        case 1:
            streamDestroy((stream->even));
            streamDestroy((stream->odd));
            free(stream);
            break;

        case 2:
            streamDestroy((stream->even));
            free(stream);
            break;
    }

    return;
}
