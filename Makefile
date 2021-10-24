mainapp: utilities.o puzmon.o
	gcc -o mainapp utilities.o puzmon.o

utilities.o: utilities.c
	gcc -c -o utilities.o utilities.c

puzmon.o: puzmon.c
	gcc -c -o puzmon.o puzmon.c