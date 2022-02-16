CFLAGS = -Wall -g -ggdb3

all:
	make clean
	make ex18
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex18 15 6 8 64 82 11 64

clean:
	rm -f ex18

g:
	gdb --args ./ex18

