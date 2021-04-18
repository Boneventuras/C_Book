#include <stdio.h>
#include <ctype.h>
#include "lib_ex_1.20-1.24.h"

int tbStop = 4;		// tabStop length

/*
gettabstop gets input from user,checks if its
a number and saves it in external tabstop
*/

int gettabstop (void)
{
	int tmp, i;
	
	printf("tabstop set to %d\n", tbStop);
	fputs("Enter single digit number (1 - 9) to set tabstop,\nenter 0 to leave preset value: ",stdout);
	while(!isdigit(tmp = getchar()))
	{
		tmp == '\n' || tmp == '\0' ? i++ :1;
		if(i < 5)
			break;
	}
	while(getchar() != '\n' && i < 5)
		;
	if(i == 5)
	{
		puts("5 incorrect inputs, returning to main\n");
		return 0;
	}
	if(tmp - 48)
	{
		tbStop = tmp - 48;
	}
	printf("tabstop set to %d\n", tbStop);
	
	return 0;
}

/*
getstring gets user input string and saves it to
str[] array
*/
int getstr (char *str, int  *index)
{
	int i;
	
	puts("Enter string to replace tabs with spaces:");
	for(i = 0; (str[i] = getchar()) != '\0' && str[i] != '\n' && i < (MAXSTRLEN - 1); i++)
		;
	str[*index = i + 1] = '\0';

	return 0;
}

/*
Exercise 1-20. Write a program detab that replaces
tabs in the input with the proper number of blanks
to space to the next tab stop. Assume a fixed set 
of tab stops, say every n columns. Should n be a
variable or a symbolic parameter?
*/

int detab (void)
{
	int i, n, ns = 0;			// index for iterations
	char str[MAXSTRLEN];	// Array for user input string
	
	puts("\033[31;106mRunning detab\033[m\n");
	gettabstop();
	getstr(str, &i);
	for(i = 0, n = 0; str[i] != '\0'; i++)
	{
		if(str[i] == '\t')
		{
			for(ns = tbStop - n%tbStop; ns > 0; ns--, n++)
			{
				putchar(' ');
			}
		}
		else
		{
			putchar(str[i]);
			n++;
		}
	}
	
	printf("num of chars after replacement %d.\n\n", n);
	
	return 0;
}

/*
Exercise 1-21. Write a program entab that replaces
strings of blanks by the minimum number of tabs and
blanks to achieve the same spacing. Use the same 
tab stops as for detab. When either a tab or a
single blank would suffice to reach a tab stop,
which should be given preference?
*/

int entab (void)
{
	// vars
	int i, n, ns;
	char str[MAXSTRLEN];
	
	// logic
	puts("\033[31;106mRunning entab\033[m\n");
	gettabstop();
	getstr(str, &i);
	printf("String length %d chars\n", i - 1);
	n = 0;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == '\t')
		{
			for(ns = tbStop - n%tbStop; ns > 0; ns--, n++)
			{
				putchar(' ');
			}
		}
		else
		{
			putchar(str[i]);
			n++;
		}
	}
	printf("num of chars after replacement %d.\n\n", n);
	return 0;
}

/*
Exercise 1-22. Write a program to ``fold'' long
input lines into two or more shorter lines after 
the last non-blank character that occurs before the
n-th column of input. Make sure your program does
something intelligent with very long lines, and if
there are no blanks or tabs before the specified
column.
*/

/*
Exercise 1-23. Write a program to remove all
comments from a C program. Don't forget to handle
quoted strings and character constants properly. C
comments don't nest.
*/

/*
Exercise 1-24. Write a program to check a C program
for rudimentary syntax errors like unmatched
parentheses, brackets and braces. Don't forget
about quotes, both single and double, escape
sequences, and comments. (This program is hard if
you do it in full generality.)
*/
