/** WARNING: This code is fresh and potentially isn't correct yet. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer){
	*out_string = calloc(1, max_buffer + 1);
	check_mem(*out_string);

	char *result = fgets(*out_string, max_buffer, stdin);
	check(result != NULL, "Input error.");

	return 0;

error:
	if(*out_string) free(*out_string);
	*out_string = NULL;
	return -1;
}

int read_int(int *out_int){
	char *input = NULL;
	int rc = read_string(&input, MAX_DATA);

	check(rc == 0, "Failed to read number.");

	*out_int = atoi(input);

	free(input);
	return 0;

error:
	if(input) free(input);
	return -1;
}

int read_scan(const char *fmt, ...){
	int i = 0;
	int rc = 0;
	int *out_int = NULL;
	char *out_char = NULL;
	char **out_string = NULL;
	int max_buffer = 0;

	va_list argp;
	va_start(argp, fmt);

	for(i = 0; fmt[i] != '\0'; i++) {
		if(fmt[i] == '%') {
			i++;
			switch(fmt[i]) {
				case '\0':
					sentinel("Invalid format, you ended with %%.");
				break;

				case 'd':
					out_int = va_arg(argp, int *);
					rc = read_int(out_int);
					check(rc == 0, "Failed to read int.");
				break;

				case 'c':
					out_char = va_arg(argp, char *);
					*out_char = fgetc(stdin);
				break;

				case 's':
					max_buffer = va_arg(argp, int);
					out_string = va_arg(argp, char **);
					rc = read_string(out_string, max_buffer);
					check(rc == 0, "Failed to read string.");
				break;

				default:
					sentinel("Invalid format.");
			}
		} else {
			fgetc(stdin);
		}
	}

	va_end(argp);
	return 0;

error:
	va_end(argp);
	return -1;
}

int print_str(char *buf, int len){
	int i;

	for(i = 0; i < len; i++){
		fputc(buf[i], stdout);	// add code
	}
	return i + 1;
}

/*
 * function to transform number into string
 * - int num	- number to convert
 * - char *buf 	- buffer for the output string
 *
 * - returns buffer char count
 */

int int_to_s(int num, char *buf){
	int temp = num, index, i = 0, neg = 0;
	char arr[18];
	if(temp == 0){
		buf[0] = '0';
		buf[1] = '\0';
		return 1;
	}
	else{
		if(temp < 0){
			neg = 1;
			//arr[index++] = '-';
			temp *= -1;
		}
		for(index = 0; temp > 0; index++){ // gal reikia nuo galo varyti iki prad=ios ir gr1=inti t1 adres1 kur baig4
			arr[index] = temp % 10 + 48;
			temp /= 10;
		}

		if(neg){
			buf[i++] = '-';
		}
		
		index--;

		for(; index >= 0; index--){
			buf[i++] = arr[index];
		}
		buf[i] = '\0';
		return i + 1;
	}
}

int print_stuff(const char *fmt, ...){
	int i = 0;
	int rc = 0;
	int *out_int = NULL;
	char *out_char = NULL;  // unused, delete at clean up
	char **out_string = NULL;
	char int_buffer[18];
	int max_buffer = 0;

	va_list argp;
	va_start(argp, fmt);

	for(i = 0; fmt[i] != '\0'; i++) {
		if(fmt[i] == '%') {
			i++;
			switch(fmt[i]) {
				case '\0':
					sentinel("Invalid format, you ended with %%.");
				break;

				case 'd':
					rc = va_arg(argp, int);
					rc = int_to_s(rc, int_buffer);	// not tested function
					rc = print_str(int_buffer, rc);		// change to print
					
					check(rc != EOF, "Failed to read int.");	// need to check if EOF is the right condition
				break;

				// change following cases to print

				case 'c':
					out_char = va_arg(argp, char *);
					*out_char = fgetc(stdin);
				break;

				case 's':
					max_buffer = va_arg(argp, int);
					out_string = va_arg(argp, char **);
					rc = read_string(out_string, max_buffer);
					check(rc == 0, "Failed to read string.");
				break;

				default:
					sentinel("Invalid format.");
			}
		} else {
			fputc(fmt[i], stdout);
		}
	}

	fputc('\0', stdout);
	va_end(argp);
	return 0;

error:
	va_end(argp);
	return -1;
}

int main(int argc, char *argv[]){
	char *first_name = NULL;
	char initial = ' ';
	char *last_name = NULL;
	int age = 0;

	printf("What's your first name? ");
	int rc = read_scan("%s", MAX_DATA, &first_name);
	check(rc == 0, "Failed first name.");

	printf("What's your initial? ");
	rc = read_scan("%c\n", &initial);
	check(rc == 0, "Failed initial.");

	printf("What's your last name? ");
	rc = read_scan("%s", MAX_DATA, &last_name);
	check(rc == 0, "Failed last name.");

	printf("How old are you? ");
	rc = read_scan("%d", &age);

	printf("---- RESULTS ----\n");
	printf("First Name: %s", first_name);
	printf("Initial: '%c'\n", initial);
	printf("Last Name: %s", last_name);
	printf("Age: %d\n", age);

	print_stuff("Age: %d\n", age);

	free(first_name);
	free(last_name);
	return 0;
error:
	return -1;
}

