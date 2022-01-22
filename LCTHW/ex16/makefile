CFLAGS = -Wall -g -ggdb3

all:
	make clean
	make ex16
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex16

clean:
	rm -f ex16

