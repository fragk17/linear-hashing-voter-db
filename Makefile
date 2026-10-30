CC = gcc

mvote : hashtable_implementation.o ziplist_implementation.o mvote.o
	$(CC) -o mvote hashtable_implementation.o ziplist_implementation.o mvote.o -lm

ziplist_implementation.o : ziplist_implementation.c
	$(CC) -c ziplist_implementation.c

hashtable_implementation.o : hashtable_implementation.c
	$(CC) -c hashtable_implementation.c

mvote.o : mvote.c
	$(CC) -c mvote.c

clean :
	rm -f *.o mvote

.PHONY: clean
