CC=gcc
CFLAGS=-g3 -Wall

inventoryManager: inventoryManager.o
	$(CC) $(CFLAGS) -o $@ $^
inventoryManager.o: inventoryManager.c inventoryManager.h

clean:
	$(RM) inventoryManager *.o