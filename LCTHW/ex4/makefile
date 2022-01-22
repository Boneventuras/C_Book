CFLAGS = -Wall -g -ggdb3

all:
	make clean
	make ex4
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex4

clean:
	rm -f ex4
