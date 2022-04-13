struct item {
    int price;
    int weight;
    char *name;
};
typedef struct item Item;

struct heap {
    int size;    // size of array
    int n;   // number of items
    long totalweight;   // total weight of all items in heap; long because trouble.in weightlimit is int limit
    Item **array;     //  array of pointers that point to items
};
typedef struct heap *Heap;

// create heap
Heap heapCreate(int size);

// insert item into heap
void itemInsert(Heap h, Item *item);

// construct a heap bottom-up
static void heapify(int n, Item **array);

// float item at position down if ratio is greater than either child's price/weight ratio
static void floatDown(int n, Item **array, int position);

// compare price/weight ratio of two items
int compRatios(Item *item1, Item *item2);

// delete item at front of array (lowest price/weight ratio)
Item* itemDelete(Heap h);

// free heap
void heapDestroy(Heap h);