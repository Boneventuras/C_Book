#ifndef lib_ex_1_20__1_24

#define lib_ex_1_20__1_24

//for test purpose
#define min( a, b) a > b ? a : b

#define MAXSTRLEN 200

/*
Exercise 1-20. Write a program detab that replaces
tabs in the input with the proper number of blanks
to space to the next tab stop. Assume a fixed set of
tab stops, say every n columns. Should n be a
variable or a symbolic parameter?
*/

int test(void);

int detab (void);
//int detab (void);

/*
Exercise 1-21. Write a program entab that replaces
strings of blanks by the minimum number of tabs and
blanks to achieve the same spacing. Use the same tab
stops as for detab. When either a tab or a single
blank would suffice to reach a tab stop, which
should be given preference?
*/

/*
Exercise 1-22. Write a program to ``fold'' long
input lines into two or more shorter lines after the
last non-blank character that occurs before the n-th
column of input. Make sure your program does
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
parentheses, brackets and braces. Don't forget about
quotes, both single and double, escape sequences,
and comments. (This program is hard if you do it in
full generality.)
*/

#endif
