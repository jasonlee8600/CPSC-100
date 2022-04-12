// Function declarations used in both table.c and expand.c

// Hash table for codewords
typedef struct table *Table;

// Include function to create hash table
Table tableCreate(void);

// Include function to free hash table strings, slot structure, and hash table itself
void tableDestroy(Table);

// Include function to insert hash key and value into table
void tableInsert(Table, const char *codeword, const char *definition);

// Looks up codeword in hash table and return string definition
    // Returns 0 if not found
const char *tableLookup(Table, const char *codeword);

// Deletes most recent definition of codeword
void tableDelete(Table, const char *codeword);
