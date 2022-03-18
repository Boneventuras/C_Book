CFLAGS = -Wall -g -ggdb3 -DNDEBUG

all: ex22_main 
	make clean
	make ex22_main
	#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex22_main
	./ex22_main

ex22_main: ex22.o

clean:
	rm -f ex22_main ex22.o

