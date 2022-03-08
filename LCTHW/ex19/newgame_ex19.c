#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "object.h"
#include "gamemech.h"

int main(int argc, char *argv[]){
	// simple way to setup the randomness
	srand(time(NULL));

	// make our map to work with
	Map *game = new_Map("The Hall of the Minotaur.");
	assert(game);

	printf("You enter the ");
	game->location->_(describe)(game->location);

	while(process_input(game)) {
	}

return 0;
}

