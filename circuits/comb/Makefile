COMPILER = gcc
CCFLAGS  = -Wall -ansi -pedantic -std=gnu99
all: comb

comb: comb.o
	$(COMPILER) $(CCFLAGS) -o comb comb.o
comb.o: comb.c comb.h
	$(COMPILER) $(CCFLAGS) -c comb.c
clean:
	rm -f comb comb.o



