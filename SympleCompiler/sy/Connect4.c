#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

bool MakeMove(char* this, int loc, bool turn)
{
	int y;
	for (y = 5; y > 0 && this[loc + y*7] != '.'; y--);
	if (y == 0)
		return false;
	
	this[loc + y*7] = turn ? 'X' : 'O';
	return true;
}

void PrintBoard(char* this)
{
	for (int i = 0; i < 7*6; i += 7)
		printf("%2i %c %c %c %c %c %c %c\n", i, this[i+0], this[i+1], this[i+2], this[i+3], this[i+4], this[i+5], this[i+6]);
}

int main()
{
	char* this = (char*)alloca(7*6);
	for (int i = 0; i < 7*6; i += 1)
		this[i] = '.';
	PrintBoard(this);
	MakeMove(this, 3, true);
	putchar('\n');
	PrintBoard(this);

	return 0;
}