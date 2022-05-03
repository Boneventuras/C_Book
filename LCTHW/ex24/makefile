INC = -I/home/$(USER)/C_Book/LCTHW/comlib
CFLAGS = -Wall -g -ggdb3 -DNDEBUG $(INC) #-rdynamic

all: 
	make clean
	make ex24
	#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex24
	./ex24

clean:
	rm -f ex24

