CC = gcc
PGNAME = mainapp
OBJS = utilities.o puzmon.o

$(PGNAME): $(OBJS)
	$(CC) -o $@ $^

utilities.o: utilities.c
	$(CC) -c -o $@ $<

puzmon.o: puzmon.c
	$(CC) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(PGNAME) $(OBJS)