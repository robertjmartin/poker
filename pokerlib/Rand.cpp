
#include <stdlib.h>
#include <time.h>
#include "rand.h"

PokerRandom::PokerRandom()
{
	srand((unsigned int)time(NULL));
}

int PokerRandom::GetRandomInt(unsigned int max)
{
	return rand() % (max+1);
}