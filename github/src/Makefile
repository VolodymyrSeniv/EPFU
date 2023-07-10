CC=g++
CFLAG=-c -Wall

all: pingu

pingu: main.o auto.o interactive.o common.o
	$(CC) main.o auto.o interactive.o common.o -o pingu -lm

main.o: main.c
	$(CC) $(CFLAG) main.c

interactive.o: interactive.c
	$(CC) $(CFLAG) interactive.c

common.o: common.c
	$(CC) $(CFLAG) common.c

auto.o: auto.c
	$(CC) $(CFLAG) auto.c

clean:
	rm -rf *o pingu
