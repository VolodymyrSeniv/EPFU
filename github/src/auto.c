
/////////////////////////////////////////////////////////////////////////
////////all the functions here are coded by Dawid Badurek
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "auto.h"
#include "common.h"


//creates empty tree
struct Tree* createEmpty()
{
	struct Tree* temp = (struct Tree*)malloc(sizeof(struct Tree));
	temp->equal = NULL;
	temp->higher = NULL;
	temp->lower = NULL;
	temp->factor = 0;
	struct Move a = { -1,-1,-1,-1 };
	temp->move = a;
	return temp;
}


void addToTree(struct Tree* tree, struct Move move, float factor)
{
	//factor is lower, put it into lower branch
	if (tree->factor > factor)
	{
		if (tree->lower == NULL)
		{
			tree->lower = createEmpty();
			tree->lower->move = move;
			tree->lower->factor = factor;
			return;
		}
		else
		{
			addToTree(tree->lower, move, factor);
			return;
		}
	}
	//factor is higher, put it into higher branch
	else if(tree->factor < factor)
	{
		if (tree->higher == NULL)
		{
			tree->higher = createEmpty();
			tree->higher->move = move;
			tree->higher->factor = factor;
			return;
		}
		else
		{
			addToTree(tree->higher, move, factor);
			return;
		}
	}
	//factors are the same, put it into equal branch
	else
	{

		if (tree->equal == NULL)
		{
			tree->equal = createEmpty();
			tree->equal->move = move;
			tree->equal->factor = factor;
			return;
		}
		else
		{
			addToTree(tree->equal, move, factor);
			return;
		}
	}
}

struct Tree* findTheHighest(struct Tree* tree)
{
	if (tree->higher == NULL)
		return tree;
	return findTheHighest(tree->higher);
}

void freeTree(struct Tree** tree)
{
	while ((*tree)->lower != NULL)
		freeTree(&(*tree)->lower);

	while((*tree)->equal!=NULL)
		freeTree(&(*tree)->equal);

	while ((*tree)->higher != NULL)
		freeTree(&(*tree)->higher);

	free(*tree);
	*tree = NULL;

}

int autoPlacement(struct GameData* data, struct Field** map, struct Player* ourPlayer, char* outputFile)
{
	//counts number of penguins on the board
	int penguins = 0;
	for (int y = 0; y < data->y; y++)
	{
		for (int x = 0; x < data->x; x++)
		{
			if (map[x][y].penguinID==ourPlayer->number)
			{
				penguins++;
			}
		}
	}
	//if there is enough penguins return result code as in .pdf file
	if (penguins == data->penguins)
		return 1;

	//tree to hold all possible moves
	struct Tree* moves = createEmpty();
	for (int y = 0; y < data->y; y++)
	{
		for (int x = 0; x < data->x; x++)
		{
			if (map[x][y].fish == 1)
			{
				//the struct Move has -1 in x_penguin and y_penguin beacause we don't use it in placement, let it have impossible value
				addToTree(moves, (struct Move) { -1, -1, x, y }, countFactor(x, y, data, map, ourPlayer));
			}
		}
	}

	struct Tree* bestMove = findTheHighest(moves);
	//no move added to bestMove so there is no place to put penguin on
	//the first move in tree is -1,-1,-1,-1 and the factor is 0 added during createEmpty() so if the bestMove returns it it means that there no place to put penguin on
	//we can assume that there is no place, because every factor is greater than 0
	if (bestMove->move.x_move == -1)
	{
		printf("ERROR: There is no place to put penguin on.\n");
		return 2;
	}

	//look for our player in data and give us points and place penguin
	for (int x = 0; x < data->players; x++)
	{
		if (data->players_data[x].number == ourPlayer->number)
		{
			data->players_data[x].result += map[bestMove->move.x_move][bestMove->move.y_move].fish;
			map[bestMove->move.x_move][bestMove->move.y_move].fish = 0;
			map[bestMove->move.x_move][bestMove->move.y_move].penguinID = ourPlayer->number;
			break;
		}
	}

	if (createOutputFile(data, map, outputFile) != 0)
		return 2;
	freeTree(&moves);
	return 0;
}

//reads data from input file, look for fields where it can put a penguin(alghorithm) - all of us
//return 0 or 1 (more in documentation to game)
int autoMovement(struct GameData* data, struct Field** map, struct Player* ourPlayer, char* outputFile)
{
	//check if the move is possible
	if (isMovePossible(ourPlayer->number, data, map) == 0)
		return 1;

	struct Tree* moves = createEmpty();
	for (int y = 0; y < data->y; y++)
	{
		for (int x = 0; x < data->x; x++)
		{
			//looks for our penguins and count factors for places it can move to
			if (map[x][y].penguinID == ourPlayer->number)
				countMoves(x,y,moves,data,map, ourPlayer);
		}
	}

	struct Tree* bestMove = findTheHighest(moves);

	//look for our player in data and give us points and move penguin
	for (int x = 0; x < data->players; x++)
	{
		if (data->players_data[x].number == ourPlayer->number)
		{
			data->players_data[x].result += map[bestMove->move.x_move][bestMove->move.y_move].fish;
			map[bestMove->move.x_move][bestMove->move.y_move].fish = 0;
			map[bestMove->move.x_move][bestMove->move.y_move].penguinID = ourPlayer->number;
			map[bestMove->move.x_penguin][bestMove->move.y_penguin].penguinID = 0;
			break;
		}
	}
	if (createOutputFile(data, map, outputFile) != 0)
		return 2;
	freeTree(&moves);
	return 0;
}

float countFactor(int x_move, int y_move, struct GameData* data, struct Field** map, struct Player* ourPlayer)
{
	float factor = FISH_FACTOR * map[x_move][y_move].fish;
	//check the field on the right
	int x = x_move;
	while (1)
	{
		x++;
		if (x < data->x)
		{
			if (map[x][y_move].fish != 0)
				factor += FISH_FACTOR * map[x][y_move].fish;
			else
			{ 
				if (map[x][y_move].penguinID != 0)
					if (map[x][y_move].penguinID != ourPlayer->number)
						factor -= PENGUIN_FACTOR;
				break;
			}
		}
		else break;
	}
	x = x_move;
	//left
	while (1)
	{
		x--;
		if (x >= 0 )
		{
			if (map[x][y_move].fish != 0)
				factor += FISH_FACTOR * map[x][y_move].fish;
			else
			{
				if (map[x][y_move].penguinID != 0)
					if(map[x][y_move].penguinID!=ourPlayer->number)
						factor -= PENGUIN_FACTOR;
				break;
			}
		}
		else break;
	}
	int y = y_move;
	//up
	while (1)
	{
		y--;
		if (y >= 0)
		{
			if (map[x_move][y].fish != 0)
				factor += FISH_FACTOR * map[x_move][y].fish;
			else
			{
				if (map[x_move][y].penguinID != 0)
					if (map[x_move][y].penguinID != ourPlayer->number)
						factor -= PENGUIN_FACTOR;
				break;
			}
		}
		else break;
	}
	y = y_move;
	//down
	while (1)
	{
		y++;
		if (y < data->y)
		{
			if (map[x_move][y].fish != 0)
				factor += FISH_FACTOR * map[x_move][y].fish;
			else
			{
				if (map[x_move][y].penguinID != 0)
					if (map[x_move][y].penguinID != ourPlayer->number)
						factor -= PENGUIN_FACTOR;
				break;
			}
		}
		else break;
	}
	//to be sure that factor won't be negative so the worst possible factor will have 0.1
	if (factor <= 0)return 0.1;
	return factor;
}
void countMoves(int x_penguin, int y_penguin,struct Tree* moves,struct GameData* data, struct Field** map, struct Player* ourPlayer)
{
	float factor = 0;
	struct Move move;

	int x = x_penguin;
	//count factors for the moves on the right
	while (1)
	{
		x++;
		if (x < data->x)
		{
			if (map[x][y_penguin].fish != 0)
			{
				factor = countFactor(x, y_penguin, data, map, ourPlayer);
				move = (struct Move){ x_penguin,y_penguin,x,y_penguin };
				addToTree(moves, move, factor);
			}
			else break;
		}
		else break;
	}

	x = x_penguin;
	//left
	while (1)
	{
		x--;
		if (x >= 0)
		{
			if (map[x][y_penguin].fish != 0)
			{
				factor = countFactor(x, y_penguin, data, map, ourPlayer);
				move = (struct Move){ x_penguin,y_penguin,x,y_penguin };
				addToTree(moves, move, factor);
			}
			else break;
		}
		else break;
	}

	int y = y_penguin;
	//up
	while (1)
	{
		y--;
		if (y >= 0)
		{
			if (map[x_penguin][y].fish != 0)
			{
				factor = countFactor(x_penguin, y, data, map, ourPlayer);
				move = (struct Move){ x_penguin,y_penguin,x_penguin,y };
				addToTree(moves, move, factor);
			}
			else break;
		}
		else break;
	}

	y = y_penguin;
	//down
	while (1)
	{
		y++;
		if (y < data->y)
		{
			if (map[x_penguin][y].fish != 0)
			{
				factor = countFactor(x_penguin, y, data, map, ourPlayer);
				move = (struct Move){ x_penguin,y_penguin,x_penguin,y };
				addToTree(moves, move, factor);
			}
			else break;
		}
		else break;
	}
}
