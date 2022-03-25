#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <execinfo.h>
#include "dbg.h"

#define CASE_FOR_DUFF_DEV(NUM) case NUM: *to++ = *from++;
//#define 

#define DUFFS_DEVICE(COUNT, LEN) {\
		int n = (COUNT + LEN - 1) / LEN;\
	\
		switch(COUNT % LEN) {\
			case 0: \
				do { *to++ = *from++;\
					case 7: *to++ = *from++;\
					case 6: *to++ = *from++;\
					case 5: *to++ = *from++;\
					case 4: *to++ = *from++;\
					case 3: *to++ = *from++;\
					case 2: *to++ = *from++;\
					case 1: *to++ = *from++;\
				} while(--n > 0);\
		}\
	}\

int normal_copy(char *from, char *to, int count){
	int i = 0;

	for(i = 0; i < count; i++) {
		to[i] = from[i];
	}

	return i;
}

int duffs_device(char *from, char *to, int count){
	{
		int n = (count + 7) / 8;
	
		switch(count % 8) {
			case 0: 
				do { *to++ = *from++;
					case 7: *to++ = *from++;
					case 6: *to++ = *from++;
					case 5: *to++ = *from++;
					case 4: *to++ = *from++;
					case 3: *to++ = *from++;
					case 2: *to++ = *from++;
					case 1: *to++ = *from++;
				} while(--n > 0);
		}
	}

	return count;
}

int zeds_device(char *from, char *to, int count){
	{
		int n = (count + 7) / 8;

		switch(count % 8) {
			case 0:
				again: *to++ = *from++;
				
				case 7: *to++ = *from++;
				case 6: *to++ = *from++;
				case 5: *to++ = *from++;
				case 4: *to++ = *from++;
				case 3: *to++ = *from++;
				case 2: *to++ = *from++;
				case 1: *to++ = *from++;
					if(--n > 0) goto again;
		}
	}

	return count;
}

int valid_copy(char *data, int count, char expects){
	int i = 0;
	
	for(i = 0; i < count; i++) {
		if(data[i] != expects) {
			log_err("[%d] %c != %c", i, data[i], expects);
			return 0;
		}
	}

	return 1;
}

int measure_time(int (*func)(char *, char *, int), char *from, char *to, int count,int *rc){
	struct timespec start, finish, delta;

	clock_gettime(CLOCK_REALTIME, &start);
	*rc = func(from, to, count);
	clock_gettime(CLOCK_REALTIME, &finish);
	delta.tv_nsec = finish.tv_nsec - start.tv_nsec;
	delta.tv_sec = finish.tv_sec - start.tv_sec;
	backtrace_symbols_fd(&func, 1, 1);	
	printf("Execution time is %d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);

	return 0;
}

int main(int argc, char *argv[]){
	char from[1000] = {'a'};
	char to[1000] = {'c'};
	int rc = 0;

	// setup the from to have some stuff
	memset(from, 'x', 1000);
	// set it to a failure mode
	memset(to, 'y', 1000);
	check(valid_copy(to, 1000, 'y'), "Not initialized right.");

	// use normal copy to
	//rc = normal_copy(from, to, 1000);
	measure_time(normal_copy, from, to, 1000, &rc);

	check(rc == 1000, "Normal copy failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "Normal copy failed.");

	// reset
	memset(to, 'y', 1000);

	// duffs version
	//rc = duffs_device(from, to, 1000);
	measure_time(duffs_device, from, to, 1000, &rc);

	check(rc == 1000, "Duff's device failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "Duff's device failed copy.");

	// reset
	memset(to, 'y', 1000);

	// my version
	//rc = zeds_device(from, to, 1000);
	measure_time(zeds_device, from, to, 1000, &rc);

	check(rc == 1000, "Zed's device failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "Zed's device failed copy.");

	return 0;

error:
	return 1;
}

