INC = -I/home/$(USER)/C_Book/LCTHW/comlib
CFLAGS = -Wall -g -ggdb3 -DNDEBUG $(INC) #-rdynamic

all: 
	make clean
	make ex25
	#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex25
	#gdb --args ex25
	./ex25

clean:
	rm -f ex25

