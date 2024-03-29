#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person{
	char *name;
	int age;
	int height;
	int weight;
};
	
struct Person Person_create(char *name, int age, int height, int weight){
	struct Person who; // = (struct Person) malloc(sizeof(struct Person));	
	assert(&who != NULL);
	
	who.name = name;//strdup(name);
	who.age = age;
	who.height = height;
	who.weight = weight;
								
	return who;
}

void Person_destroy(struct Person *who)
{
	assert(who);
	
	printf("who ne = %s\n", who->name);

	free(who->name);
	free(who);
}

void Person_print(struct Person who)
{
	printf("Name:\t%s\n", who.name);
	printf("\t\tage:\t%d\n", who.age);
	printf("\t\theight:\t%d\n", who.height);
	printf("\t\tweight:\t%d\n\n", who.weight);
}

int main()
{
	struct Person joe = Person_create("Joe Alex", 32, 64, 140);
	struct Person frank = Person_create("Frank Blank", 20, 72, 180);
				
	printf("Joe is at memory location %p\n", &joe);
	Person_print(joe);
	printf("Frank is at memory location %p\n", &frank);
	Person_print(frank);
								
	joe.age += 20;
	joe.height -= 2;
	joe.weight += 20;
	Person_print(joe);
	
	frank.age += 20;
	frank.weight += 40;
	Person_print(frank);

	//Person_destroy(&joe);
	//Person_destroy(&frank);
	Person_destroy(NULL);
	
	return 0;
}

