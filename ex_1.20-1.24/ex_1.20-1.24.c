#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib_ex_1.20-1.24.h"
#include "lib_ex_1.20-1.24.c"

int main()
{
	// Pasidometi ASCII escape chars for color text
	char c, tmp;
	
	while(1)
	{
		//test();
		puts("Availible functions for strings:");
		puts("\t1 - detab (replaces tabs with spaces)");
		puts("\t2 - entb (replaces multiple spaces with tabs)");
		puts("\t0 - to exit");
		puts("\033[35;106mSelect one function by entering its number:\033[m");
		while((c = getchar()) == '\n' && c == '\0')
			;
		if((tmp = getchar()) != '\n' || tmp != '\0')
		{
			putchar(tmp);
		}
		if(isdigit(c))
		{
			switch(c)
			{
				case '1':
				detab();
				break;
				
				case '2':
				puts("\n\033[31;106mentab\033[m\n");
				test();
				break;
				
				case '0':
				puts("Exit?\ny - for yes\t\tenter any other vlues to continue");
				while((c = getchar()) == '\n' || c == '\0')
					;
				if(c == 'y' || c == 'Y')
					return 0;
				while((c = getchar()) != '\n' && c != '\0')
					;
				putchar('\n');
			}
		}
		else
		{
			puts("\n\033[31;106mNot valid entry, please select again\033[m\n");	
		}
	}
}
