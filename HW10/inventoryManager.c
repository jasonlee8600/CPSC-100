#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "inventoryManager.h"


// some functions from lecture notes 5.10.6

/* compute child 0 or 1 */
#define Child(x, dir) (2*(x)+1+(dir))
#define INITIAL_SIZE (100)


// create heap
Heap heapCreate(int size) 
{
    Heap h = malloc(sizeof(*h));

    h->size = size;
    h->n = 0;
    h->totalweight = 0;
    h->array = calloc(size, sizeof(Item*));

    return h;
}



// construct a heap bottom-up
static void heapify(int n, Item **array)
{
    int i;

    for(i = n-1; i >= 0; i--) {
        floatDown(n, array, i);
    }
}



// float item at position down if ratio is greater than either child's price/weight ratio
static void floatDown(int n, Item **array, int position)
{
    Item *x;

    // save original value once 
    x = array[position];

    for(;;) {
                                    // if right child has lower ratio than left child
        if(Child(position, 1) < n && compRatios(array[Child(position, 1)], array[Child(position, 0)])) 
        {
            // if right child has lower ratio than parent item x
            if(compRatios(array[Child(position, 1)], x)) 
            {
                // swap with right child
                array[position] = array[Child(position, 1)];
                position = Child(position, 1);
            } else 
            {
                // item x has smaller ratio than both kids
                break;
            }
        } 
                                        // if left child has lower ratio than parent item x
        else if(Child(position, 0) < n && compRatios(array[Child(position, 0)], x)) 
        {
            // swap with left child
            array[position] = array[Child(position, 0)];
            position = Child(position, 0);
        } 
        else 
        {
            break;
        }
    }

    array[position] = x;
}



// see if item1 has lower ratio than item2
int compRatios(Item *item1, Item *item2)
{
    // ratio1 = price1/weight1 
    // ratio2 = price2/weight2 
    // ratio1 < ratio2 if...
        // price1 * weight2 < price2 * weight1

    if((item1->price * item2->weight) < (item2->price * item1->weight))
    {
        return 1;   // true, ratio1 < ratio2
    }
    else
    {
        return 0;
    }
}



// insert item into heap
void itemInsert(Heap h, Item *item) 
{
    // store item in next array index
    h->array[h->n] = item;
    // increase number of items in heap by 1
    h->n++;
    // add item's weight to heap's totalweight
    h->totalweight += item->weight;

    // heapify again to maintain ratio invariant
    heapify(h->n, h->array);
}



// delete item at front of array (lowest price/weight ratio)
Item* itemDelete(Heap h)
{
    // set item to delete to first item in array (lowest ratio)
    Item* deleted;
    deleted = h->array[0];

    // subtract deleted item's weight from heap's totalweight
    h->totalweight -= deleted->weight;
    // move item at end of array to front
    h->array[0] = h->array[h->n - 1];
    // set end of array spot to null where we just pulled from
    h->array[h->n - 1] = 0;
    // decrease number of elements in array by 1
    h->n--;

    // reconstruct heap to maintain ratio invariant
    heapify(h->n, h->array);

    return deleted;
}


// free heap
void heapDestroy(Heap h)
{
    for (int i = 0; i < h->n; i++)
    {
        free(h->array[i]->name);
        free(h->array[i]);
    }
    free(h->array);
    free(h);
}



int main() {

    // priority queue heap as array (root/first index is min)
    Heap h = heapCreate(INITIAL_SIZE);

    // inventory weight limit
    int weightlimit;
    if(scanf("%d", &weightlimit) < 1)
    {
        return 1;
    }


    // price and weight
    int p, w;
    // item name
    char *s = NULL;

    while(scanf("%d %d %m[^\n]", &p, &w, &s) == 3)
    {
        Item* inserted = malloc(sizeof(Item));
        inserted->price = p;
        inserted->weight = w;
        inserted->name = s;
        itemInsert(h, inserted);

        while(h->totalweight > weightlimit)
        {
            Item* deleted = itemDelete(h);
            printf("%d %d %s\n", (deleted->price), (deleted->weight), (deleted->name));
            free(deleted->name);
            free(deleted);
        }
    }

    heapDestroy(h);

    return 0;
}
