CFLAGS = -Wall -g -ggdb3

all: ex19
	make clean
	make ex19
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex19

n: newgame_ex19
	make clean
	make newgame_ex19
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./newgame_ex19

ex19: object.o

newgame_ex19: object.o gamemech.o

clean:
	rm -f ex19 newgame_ex19 object.o gamemech.o

