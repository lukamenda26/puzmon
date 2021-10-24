CC = gcc
PGNAME = mainapp
OBJS = utilities.o enemyTurn.o puzmon.o

$(PGNAME): $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(PGNAME) $(OBJS)