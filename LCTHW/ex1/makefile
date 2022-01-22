CFLAGS = -Wall -g -ggdb3

all:
	make clean
	make ex1 
	valgrind -s ./ex1

clean:
	rm -f ex1

del.out: del.c
	echo /////////////////////////////////////
	make clean
	gcc del.c -o del.out

test.out: test.c
	echo "/////////////////////////////////////"
	make clean
	gcc test.c -o test.out

run:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./del.out z.txt

