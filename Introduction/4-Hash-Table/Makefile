COMPILER = gcc
CCFLAGS  = -Wall -ansi -pedantic -std=gnu99
all: fourth

fourth: fourth.o
	$(COMPILER) $(CCFLAGS) -o fourth fourth.o
third.o: fourth.c fourth.h
	$(COMPILER) $(CCFLAGS) -c fourth.c 
clean:
	rm -f fourth fourth.o
