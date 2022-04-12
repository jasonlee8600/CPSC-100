#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "table.h"

// Code altered from CS 223 lecture notes Ch. 5.6

// structs for hash table and entries

struct elt {
    struct elt *next;
    char *codeword;   
    char *definition; 
};

struct table {
    int size;    // size of hash table
    int n;    // number of stored elements in hash table
    struct elt **table;   // table of linked lists
};


// Intitalize hash table size and multiplier
#define INITIAL_SIZE (32)
#define MULTIPLIER (2)
#define MAX_LOAD_FACTOR (1)


// Function to create hash table
static Table tableCreateInternal(int size)
{
    Table d = malloc(sizeof(Table));
    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = calloc(d->size, sizeof(struct elt *));
    assert(d->table != 0);

    for(int i = 0; i < d->size; i++) {
        d->table[i] = 0;
    }

    return d;
}


Table tableCreate(void)
{
    return tableCreateInternal(INITIAL_SIZE);
}



// Function to free hash table and its contents
void tableDestroy(Table d)
{
    // walk through all table entries and delete them
    struct elt *e;
    struct elt *next;

    for(int i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;
            free(e->codeword);
            free(e->definition);
            free(e);
        }
    }

    free(d->table);
    free(d);
}



// Fowler-Noll-Vo hash function from CS 223 lecture notes
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

uint64_t hash_function(const char *codeword)
{
    uint64_t hash = FNV_OFFSET_BASIS_64;

    for(size_t i = 0; codeword[i] != '\0'; i++) {
        hash = hash ^ codeword[i];
        hash *= FNV_PRIME_64;
    }

    return hash;
}



// Function to make new hash table if old one become too filled
static void tableGrow(Table d) 
{
    struct table swap;
    struct elt *e;

    // Creates new hash table double the size of old
    Table d2 = tableCreateInternal(d->size * MULTIPLIER);

    for (int i = 0; i < d->size; i++)
    {
        for (e = d->table[i]; e != 0; e = e->next)
        {
            // copies everything from original table to new table
            tableInsert(d2, e->codeword, e->definition);
        }
    }

    // swap guts of d and d2 then free d2
    swap = *d;
    *d = *d2;
    *d2 = swap;

    tableDestroy(d2); 
}



// Function to insert codeword and definition into hash table
void tableInsert(Table d, const char *codeword, const char *definition)
{
    struct elt *e;
    unsigned long hash;

    assert(codeword);
    assert(definition);

    e = malloc(sizeof(*e));
    assert(e);

    // copy codeword and definition into elt *e
    e->codeword = strdup(codeword);
    e->definition = strdup(definition);

    // hash the codeword modulo size of table
    hash = hash_function(codeword) % d->size;


    e->next = d->table[hash];
    d->table[hash] = e;
    d->n++; // increase number of stored elements in hash table by 1

    // if stored elements is greater than or equal to size of table, 
        // grow table size and copy contents
    if (d->n >= d->size * MAX_LOAD_FACTOR)
    {
        tableGrow(d);
    }
}



// Function to lookup codeword in hash table and return definition or 0 if not found
const char *tableLookup(Table d, const char *codeword)
{
    struct elt *e;
    uint64_t hash = hash_function(codeword) % d->size;

    // checks each hash table slot
    for (e = d->table[hash]; e != 0; e = e->next)
    {
        // if current hash table slot's codeword is same as one we are looking for
            // return its definition
        if (!strcmp(e->codeword, codeword))
        {
            return e->definition;
        }
    }

    // didn't find it in hash table
    return 0;
}



// Deletes most recent definition of codeword
void tableDelete(Table d, const char *codeword)
{
    struct elt **prev;          // what to change when elt is deleted
    struct elt *e;              // what to delete 
    uint64_t hash = hash_function(codeword) % d->size;

    for(prev = &(d->table[hash]); 
        *prev != 0; 
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->codeword, codeword)) {
            // got it
            e = *prev;
            *prev = e->next;

            free(e->codeword);
            free(e->definition);
            free(e);

            return;
        }
    }
}