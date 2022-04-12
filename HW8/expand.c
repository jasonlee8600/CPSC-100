#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


#include "table.h"


int main(int argc, char **argv)
{
    // Create hash table (create function)
    Table t = tableCreate();

    // pointer to current word being read by scanf
    char *word;
    // to store codeword
    char *word2;

    // counter set to 1 when reading "DEFINE", 
        // to 2 when inserting defintion words one at a time,
        // and back to 0 when reading "STOP"
    int counter = 0; 
    
    // Loop to read each word from stdin until end of input
    while (scanf("%ms", &word) == 1)
    {
        if (strcmp(word, "DEFINE") == 0)
        {
            counter = 1;
            continue;       // continue to next while loop iteration
        }

        if (counter == 1) // reading codeword
        {
            word2 = word;   // CHECK IF THIS WORKS
            counter = 2; // set to 2 so next words read are inserted into definition in hash table

            if ((tableLookup(t, word)) != 0) // if a definition exists, delete it
            {
                tableDelete(t, word);
            }
            
            continue; // continue to next while loop iteration
        }

        if (strcmp(word, "STOP") == 0) 
        {
            counter = 0;
            continue; // continue to next while loop iteration
        }

        if (counter == 2) // reading a definition word
        {
            tableInsert(t, word2, word); // CHECK TO SEE IF IT INSERTS EACH STRING OR REPLACES
            continue; // continue to next while loop iteration
        }

        if (counter == 0)
        {
            if ((tableLookup(t, word)) != 0)
            {
                puts(tableLookup(t, word)); // print defintion if it exists
            }

            else 
            {
                puts(word); // else just print the word we are reading
            }
        }
    }

    // Free hash table
    tableDestroy(t);

    return 0;
}
