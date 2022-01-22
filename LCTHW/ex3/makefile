CFLAGS = -Wall -g -ggdb3

all:
	make clean
	make ex3
	valgrind -s ./ex3
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex3

clean:
	rm -f ex3

