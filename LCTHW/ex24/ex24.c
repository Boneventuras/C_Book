#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

#define MAX_DATA 100
#define MAX_INT 11
#define MAX_FLOAT 19

typedef enum EyeColor {
	BLUE_EYES, GREEN_EYES, BROWN_EYES,
	BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
	"Blue", "Green", "Brown", "Black", "Other"};

typedef struct Person {
	int age;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
	EyeColor eyes;
	float income;
} Person;

// Function to get integer from stdin, int range from -1 999 999 999 to 1 999 999 999

int getint(int *var){
	char s[MAX_INT];
	char *p = fgets(s, MAX_INT, stdin);
	
	if(p){
		for(int i = 0; i < MAX_INT; i++){
			if((i == 10 && (*p > '1')) || (i == 11 && (*p == '-' && *(p + 1) > '1'))){
				puts("Number out of range, please enter number between \
						1 999 999 999 and -1 999 999 999.");
				*var = 0;
				return 0;
			}
			if(*(p + i) == '\n' || *(p + i) == EOF){
				*var = atoi(p);
				return i;
			}
		}
	}
	*var = 0;
	return 0;
}

// Function to get float from stdin, flaot possible greated ten -2 000 000 000 
// and less then 2 000 000 000

int getfloat(float *var){
	char s[MAX_FLOAT]; // Temp array
	char p[MAX_FLOAT]; // Array to save input
	int point_i = -1, i, len, count = 0;
	float temp;

	if(fgets(p, MAX_FLOAT, stdin)){
		// loop to get separator position and length of number
		for(i = 0; i < MAX_FLOAT; i++){
			if(( (*(p + i) == '.') || (*(p + i) == ',') ) && (point_i < 0) && point_i == -1){
				point_i = i;
			}else if(*(p + i) == '\n' || *(p + i) == EOF){
				len = i;
				break;
			}
		}

		if(point_i <= -1){
			if((len == 10 && (*p > '1')) || (len == 11 && (*p == '-' && *(p + 1) > '1'))){
				puts("Number out of range, please enter number between \
						1 999 999 999 and -1 999 999 999.");
				*var = 0;
				return 0;
			}
			*var = (float)atoi(p);
			return len;
		}else if(point_i > -1){ // when the comma is the first char
			memset(s, 0, MAX_FLOAT);
			if(point_i == 0 && len >= 10){
				memcpy(s, p, len);		
				*var = (float)atoi(s + 1);

				while(var > 0 && count++ <= len){
					*var = *var / 10;
				}
			}else if(point_i <= 10 && len <= MAX_FLOAT){ // when comma is one of the first 10 chars in input
				if((len == 11 && (*p > '1')) || (len == 12 && (*p == '-' && *(p + 1) > '1'))){
					puts("Number out of range, please enter number between \
							1 999 999 999 and -1 999 999 999.");
					*var = 0;
					return 0;
				}else{
					*var = (float)atoi(p);
					count = 0;
					memcpy(s, p + point_i + 1, len - point_i - 1 < 10 ? len - point_i - 1 : 9);
					temp = (float)atoi(p + point_i + 1);
					while(temp > 0 && count++ < len - point_i - 1){
						temp = temp / 10;
					}
					return *var += temp;
				}
			}else{
				puts("Undefined behavior");
			}
		}
	}

	return 0;
}

int main(int argc, char *argv[]){
	Person you = {.age = 0};
	int i = 0;
	char *in = NULL;

	printf("What's your First Name? ");
	in = fgets(you.first_name, MAX_DATA-1, stdin);
	check(in != NULL, "Failed to read first name.");

	printf("What's your Last Name? ");
	in = fgets(you.last_name, MAX_DATA-1, stdin);
	check(in != NULL, "Failed to read last name.");

	printf("How old are you? ");
//	int rc = fscanf(stdin, "%d", &you.age);
	int rc = getint(&you.age);
	check(rc > 0, "You have to enter a number.");

	printf("What color are your eyes:\n");
	for(i = 0; i <= OTHER_EYES; i++) {
		printf("%d) %s\n", i+1, EYE_COLOR_NAMES[i]);
	}
	printf("> ");

	int eyes = -1;
//	rc = fscanf(stdin, "%d", &eyes);
	rc = getint(&eyes);
	check(rc > 0, "You have to enter a number.");

	you.eyes = eyes - 1;
	check(you.eyes <= OTHER_EYES && you.eyes >= 0, "Do it right, that's not an option.");

	printf("How much do you make an hour? ");
//	rc = fscanf(stdin, "%f", &you.income);
	rc = getfloat(&you.income);
	check(rc > 0, "Enter a floating point number.");

	printf("----- RESULTS -----\n");

	printf("First Name: %s", you.first_name);
	printf("Last Name: %s", you.last_name);
	printf("Age: %d\n", you.age);
	printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
	printf("Income: %f\n", you.income);

	return 0;

error:
	return -1;
}

