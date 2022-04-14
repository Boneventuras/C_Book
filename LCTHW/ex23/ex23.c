#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <execinfo.h>
#include "dbg.h"

// Macros to create 32 case Duff device

#define CASE_DUFF(NUM) case (NUM): *to++ = *from++

#define SEVEN_CASE(NUM) \
	CASE_DUFF(NUM);\
	CASE_DUFF(NUM - 1);\
	CASE_DUFF(NUM - 2);\
	CASE_DUFF(NUM - 3);\
	CASE_DUFF(NUM - 4);\
	CASE_DUFF(NUM - 5);\
	CASE_DUFF(NUM - 6);

#define EIGHT_CASE(NUM)\
	CASE_DUFF(NUM);\
	SEVEN_CASE(NUM - 1)

#define DUFFS_DEVICE(COUNT) {\
		int n = (COUNT + 31) / 32;\
	\
		switch(COUNT % 32) {\
			case 0: \
				do { *to++ = *from++;\
					EIGHT_CASE(31);\
					EIGHT_CASE(23);\
					EIGHT_CASE(15);\
					SEVEN_CASE(7);\
				} while(--n > 0);\
		}\
	}\

int macro_test (char *from, char *to, int count){
	DUFFS_DEVICE(count);
	
	return count;
}

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

// Function to measure time of functions defined in this file

int meas_time(int (*func)(char *, char *, int), char *from, char *to, int count, int *rc){
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

// Function to measure memcopy and memmove time
 
int meas_time_mcp_mmv(void* (*func)(void * restrict, const void * restrict, size_t), char *from, char *to, int count, char *fname){
	struct timespec start, finish, delta;

	clock_gettime(CLOCK_REALTIME, &start);
	func(to, from, count);
	clock_gettime(CLOCK_REALTIME, &finish);
	delta.tv_nsec = finish.tv_nsec - start.tv_nsec;
	delta.tv_sec = finish.tv_sec - start.tv_sec;
	printf("%s:\n", fname);
	printf("Execution time is %d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);

	return 0;
}

// Function to measure Duff device macro time

int meas_time_macro(int (*func)(char *, char *, int), char *from, char *to, int count, int *rc){

	struct timespec start, finish, delta;

	clock_gettime(CLOCK_REALTIME, &start);
	*rc = func(from, to, count);
	clock_gettime(CLOCK_REALTIME, &finish);
	delta.tv_nsec = finish.tv_nsec - start.tv_nsec;
	delta.tv_sec = finish.tv_sec - start.tv_sec;
	printf("Execution time is %d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);

	return 0;
}

int main(int argc, char *argv[]){
	char from[1000] = {'a'};
	char to[1000] = {'c'};
	int rc = 0;
	struct timespec start, finish, delta;

	clock_gettime(CLOCK_REALTIME, &start);
	
	// setup the from to have some stuff
	memset(from, 'x', 1000);

	clock_gettime(CLOCK_REALTIME, &finish);
	delta.tv_nsec = finish.tv_nsec - start.tv_nsec;
	delta.tv_sec = finish.tv_sec - start.tv_sec;
	puts("memset:");
	printf("Execution time is %d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);
	
	// set it to a failure mode
	memset(to, 'y', 1000);
	check(valid_copy(to, 1000, 'y'), "Not initialized right.");

	// use normal copy to
	//rc = normal_copy(from, to, 1000);
	meas_time(normal_copy, from, to, 1000, &rc);

	check(rc == 1000, "Normal copy failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "Normal copy failed.");

	// reset
	memset(to, 'y', 1000);

	// duffs version
	//rc = duffs_device(from, to, 1000);
	meas_time(duffs_device, from, to, 1000, &rc);

	check(rc == 1000, "Duff's device failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "Duff's device failed copy.");

	// reset
	memset(to, 'y', 1000);

	// my version
	//rc = zeds_device(from, to, 1000);
	meas_time(zeds_device, from, to, 1000, &rc);

	check(rc == 1000, "Zed's device failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "Zed's device failed copy.");

	memset(to, 'y', 1000);

	meas_time_mcp_mmv(memcpy, from, to, 1000, "memcpy");
	check(valid_copy(to, 1000, 'x'), "memcpy failed copy.");

	memset(to, 'y', 1000);

	meas_time_mcp_mmv(memmove, from, to, 1000, "memmove");
	puts("Before check");
	check(valid_copy(to, 1000, 'x'), "memcpy failed copy.");


	// set it to a failure mode
	memset(to, 'y', 1000);
	check(valid_copy(to, 1000, 'y'), "Not initialized right.");

	// use normal copy to
	//rc = normal_copy(from, to, 1000);
	puts("macro_test time:");
	meas_time_macro(macro_test, from, to, 1000, &rc);

	check(rc == 1000, "macro_test copy failed: %d", rc);
	check(valid_copy(to, 1000, 'x'), "macro_test copy failed.");

	return 0;

error:
	return 1;
}

