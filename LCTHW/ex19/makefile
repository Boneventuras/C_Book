CFLAGS = -Wall -g -ggdb3

all: ex19
	make clean
	make ex19
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex19

ex19: object.o

clean:
	rm -f ex19 object.o

