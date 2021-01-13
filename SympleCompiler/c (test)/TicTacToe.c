#define _NO_CRT_STDIO_INLINE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define Board char*

Board CreateBoard()
{
	Board this = (Board)malloc(9);
	for (int i = 0; i < 9; i = i + 1)
		this[i] = '#';
		
	return this;
};

void PrintBoard(Board this)
{
	printf("%c | %c | %c\n", this[0], this[1], this[2]);
	printf("%c | %c | %c\n", this[3], this[4], this[5]);
	printf("%c | %c | %c\n", this[6], this[7], this[8]);
};

// 1 | . | .
// . | 2 | .
// . | . | 3

int GetWinner(Board this)
{
	char c;
	
	if ((c = this[0]) != '#')
	{
		if (this[1] == c && this[2] == c)
			return c == 'X' ? 1 : 2;
		else if (this[3] == c && this[6] == c)
			return c == 'X' ? 1 : 2;
		else if (this[4] == c && this[8] == c)
			return c == 'X' ? 1 : 2;
	}
	if ((c = this[4]) != '#')
	{
		if (this[3] == c && this[5] == c)
			return c == 'X' ? 1 : 2;
		else if (this[1] == c && this[7] == c)
			return c == 'X' ? 1 : 2;
		else if (this[2] == c && this[6] == c)
			return c == 'X' ? 1 : 2;
	}
	if ((c = this[8]) != '#')
	{
		if (this[2] == c && this[5] == c)
			return c == 'X' ? 1 : 2;
		else if (this[6] == c && this[7] == c)
			return c == 'X' ? 1 : 2;
	}
	
	return 0;
};

int main()
{
	bool playerTurn = true;
	Board board = CreateBoard();
	
	while (!GetWinner(board))
	{
		PrintBoard(board);
		int turn = 0;
		
		while (turn < 1 || turn > 9)
		{
			printf("Player '%c', pick a number from 1-10...\n", playerTurn ? 'X' : 'O');
			scanf_s("%i", &turn);
		};
		
		board[turn - 1] = playerTurn ? 'X' : 'O';
		playerTurn = !playerTurn;
	};
	
	PrintBoard(board);
	GetWinner(board);
};