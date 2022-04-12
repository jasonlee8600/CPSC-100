CC=gcc
CFLAGS=-g3 -Wall

expand: expand.o table.o
	$(CC) $(CFLAGS) -o $@ $^
expand.o: expand.c table.h
table.o: table.c table.h

clean:
	$(RM) expand *.o