CFLAGS = -Wall -g -ggdb3 -DNDEBUG -rdynamic

all: 
	make clean
	make ex23
	#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex23
	./ex23

clean:
	rm -f ex23

