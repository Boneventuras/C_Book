CFLAGS = -Wall -g -ggdb3

all:
	make clean
	make ex17
#	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./ex17 z.txt c 4 8
	./ex17 z.txt c 4 7 

#	xxd -c 8 z.txt

sg:
	make clean
	make singleGlobal_ex17
	./singleGlobal_ex17 x.txt c 4 7

clean:
	rm -f ex17 singleGlobal_ex17 z.txt x.txt

g_17_s:
	gdb --args ./ex17 z.txt s 0 ABC abc

