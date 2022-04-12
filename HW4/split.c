#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "split.h"

// check all str functions for runtime!!
// try manually copying things instead of strcat or strcpy
// joinMany " " 100000 cat in the hat... not working

// at end, copy code to local split.c (overwrite)

char *join(char **a, int c) {

    // if no input
    if (a[0] == NULL)
    {
        char *answer = calloc(1, sizeof(char));
        answer[0] = '\0';
        return answer;
    }
    
    // if c is '\0' == int 0
    if (c == 0) 
    {
        // start at 1 to account for '\0' at end of joined string
        int size = 1;

        // adds size of each a[i] until reaching null pointer in char **a
        // ./testSplit join '' ab cd efg    a[2] == "efg"  strlen(a[2]) == 3
        for (int i = 0; a[i] != NULL; i++)
        {
            size += strlen(a[i]);
        }

        // answer array with size to hold joined string (w/ no separators between the string args)
        char *answer = calloc(size, sizeof(char));

        // copy each string in char **a into answer using strcat
        for (int i = 0; a[i] != NULL; i++)
        {
            strcat(answer, a[i]);
        }

        // sets last index to null
        answer[size - 1] = '\0';

        return answer;
    }

    else
    {
        // changes int c to a char x
        const char x = c;

        // start at 0 because length of **a accounts for number of times we need separator and '\0' at end
        int size2 = 0;


        // adds size of each a[n] until reaching null pointer in char **a
        // +1 for each iteration because **a array size accounts for number of times we need separator and '\0' at end
        // ./testSplit join : ab cd efg    a[2] == "efg"  strlen(a[2]) == 3
        for (int i = 0; a[i] != NULL; i++)
        {
            size2 += strlen(a[i]) + 1;
        }

        // answer array with size to hold joined string
        char *answer2 = calloc(size2, sizeof(char));


        // copy char's from **a into answer2
        int index = 0;

        for (int i = 0; a[i] != NULL; i++)
        {
            for (int j = 0; a[i][j] != '\0'; j++)
            {
                answer2[index] = a[i][j];
                index++;
            }
            
            // put separator after substring
            answer2[index] = x;

            // move to next answer2 index
            index++;
        }
        

        // sets last index to null
        answer2[size2 - 1] = '\0';
        
        return answer2;

    }
}



char **split(const char *s, int c) {
    
    // if c is '\0'
    if (c == 0) 
    {
        // use length so strlen(s) doesn't need to be computed multiple times
        int length = strlen(s);
        
        // ex: input string "a::bc" needs result to have size for...
            // input string (strlen(s))
            // + 1 for '\0' at end of input string that strlen(s) doesn't account for
            // 2 pointers (2 * sizeof(s)) for...
                // answer[0] (pointer that points to string)
                // answer[1] (null pointer)

        // answer == pointer to pointer array
        char **answer = calloc(2, sizeof(char*));

        // allocate space for string answer[0] points to
        answer[0] = calloc((length + 1), sizeof(char));
        
        // copy string into where answer[0] points
        strcpy(answer[0], s);

        // sets last index to null
        answer[0][length] = '\0';

        return answer;
    }

    else
    {
        // changes int c to char x
        const char x = c;

        // increase by 1 for each occurence of the separator in the spring
        int size = 0;

        // use length so strlen(s) doesn't need to be computed multiple times
        int length = strlen(s);

        // counts each occurence of separator
        for (int i = 0; i < length; i++)
        {
            if (s[i] == x)
            {
                size++;
            }
        }

        // answer == pointer to pointer array
        // (2 + size) * sizeof(pointer) bc...
            // 2 takes care of answer2[0] (that holds first split string) and NULL pointer
            // size accounts for the extra pointers needed for each occurence of separator
        char **answer2 = calloc((2 + size), sizeof(char*));


        // assign whole string to a[0] if no occurence of separator at all in string
        if (size == 0)
        {
            answer2[0] = calloc((length + 1), sizeof(char));

            // copy string into where answer[0] points
            strcpy(answer2[0], s);

            // sets last index to null
            answer2[0][length] = '\0';

            return answer2;
        }
        

        // counter for number of char's we need to allocate memory for in each substring
        int count = 0;

        // counter to know what char we are on in string s
        int place = 0;

        // iterate through array pointer
        for (int i = 0; i < size; i++)
        {   
            // moves down string until separator, counting for chars in substring and keeping track of place in original string
            while (s[place] != x)
            {
                count++;
                place++;
            }
            
            // + 1 for '\0'
            answer2[i] = calloc((count + 1), sizeof(char));

            // reset count
            count = 0;

            // skips over separator to next char in string
            place++;
        } 



        // counter for number of char's we need to allocate memory for last substring specifically
        int count2 = 0;
        while (s[place] != '\0')
        {
            count2++;
            place++;
        }
        
        // calloc space for last substring
        answer2[size] = calloc((count2 + 1), sizeof(char));



        // another var to keep track of place in original string
        int place2 = 0;


        for (int i = 0; i < size; i++)
        {            
            // substring index value
            int j = 0;

            // assigns char's to calloc'ed substring spaces until separator
            while (s[place2] != x)
            {
                answer2[i][j] = s[place2];
                j++;
                place2++;
            }

            // set last index of substring to null
            answer2[i][j] = '\0';

            // skips over separator to next char in string
            place2++;
        }

        // do same process for last substring
        int p = 0;   // last substring index value

        // assigns char's to calloc'ed substring spaces until end of string
        while (s[place2] != '\0')
        {
            answer2[size][p] = s[place2];
            p++;
            place2++;
        }

        // sets last index of substring to null
        answer2[size][p] = '\0';

        return answer2;
    }
}



void freeSplit(char **a) {

    // frees each substring a[i] pointer points to
    for (int i = 0; a[i] != NULL; i++)
    {
        free(a[i]);
    }

    // frees pointer array
    free(a);

    return;
}
