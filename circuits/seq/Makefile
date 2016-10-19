COMPILER = gcc
CCFLAGS  = -Wall -ansi -pedantic -std=gnu99
all: seq

seq: seq.o
	$(COMPILER) $(CCFLAGS) -o seq seq.o
seq.o: seq.c seq.h
	$(COMPILER) $(CCFLAGS) -c seq.c
clean:
	rm -f seq seq.o



