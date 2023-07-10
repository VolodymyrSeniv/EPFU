#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"


//Dawid Badurek
void clearConsole()
{
#ifdef WINDOWS
	system("cls");
#endif
#ifdef LINUX
	printf("\033[H\033[J");
#endif
#ifdef MAC
	system("clear");
#endif
}

/*Creates an external .txt file, containing current state of the map, and each player's data: id, number, and result - Ania Krawczuk */
int createOutputFile(struct GameData* data,struct Field** map,char* name)
{
	FILE* file = NULL;
	file = fopen(name, "w");
	if (file == NULL)
	{
		printf("ERROR: Couldn't create output file.\n");
		return 2;
	}
	fprintf(file, "%d %d\n", data->y, data->x);

	for (int i = 0; i < data->y; i++)
	{
		for (int j = 0; j < data->x; j++)
		{
			fprintf(file,"%d%d ", map[j][i].fish, map[j][i].penguinID);
		}
		fprintf(file,"\n");
	}

	for (int i = 0; i < data->players; i++) fprintf(file, "%s %d %d\n", data->players_data[i].id, data->players_data[i].number, data->players_data[i].result);

	fclose(file);
	return 0;
}



//Vladimir Seniv
int fileSize(FILE* file)
{
	int file_size = 0;
	while (fgetc(file) != EOF)
		file_size++;
	fseek(file, 0, SEEK_SET);
	return file_size;
}
//Vladimir Seniv
TFormattedData split_data(char* raw_data)
{
	char delimiters[3] = " \n";
	char** splitted_data = (char**)malloc(2 * sizeof(char*));
	int a, b;
	splitted_data[0] = strtok(raw_data, delimiters);
	a = atoi(splitted_data[0]);
	splitted_data[1] = strtok(NULL, delimiters);
	b = atoi(splitted_data[1]);
	splitted_data = (char**)realloc(splitted_data, (a * b + 29) * sizeof(char*));
	int i;
	for (i = 2; 1; i++)
	{
		char* temp = strtok(NULL, delimiters);
		if (temp == NULL)
			break;
		splitted_data[i] = temp;
	}
	TFormattedData result;
	result.size = i;
	result.tokens = splitted_data;
	return result;
}
//Vladimir Seniv
ERRORS check_formatted(TFormattedData splitted_data)
{
	/*a, b, n
	2 + a * b + 3 * n*/
	int height, width, j = 0;
	char c;
	char* p;
	for (int i = 0; i < splitted_data.size; i++)
	{
		if (i == 0)
		{
		
		height = atoi(splitted_data.tokens[i]);
		if (height == 0)return BAD_DIMENSIONS;
		}
		else if (i == 1)
		{
			width = atoi(splitted_data.tokens[i]);
			if (width == 0)return BAD_DIMENSIONS;
		}
		else if (i < height * width + 2)
		{
			c = splitted_data.tokens[i][0];
			if (c < '0' || c > '3')
				return BAD_CELL;
			c = splitted_data.tokens[i][1];
			if (c < '0' || c > '9')
				return BAD_CELL;
			c = splitted_data.tokens[i][2];
			if (c != '\0')
				return BAD_CELL;
		}
		else
		{
			j = i - (height * width + 2);
			if (j % 3 == 0)
				for (p = splitted_data.tokens[i]; *p != '\0'; p++)
					if (!isalpha(*p))
						return BAD_PLAYER_NAME;
			if (j % 3 == 1)
			{
				if (!isdigit(splitted_data.tokens[i][0]) || splitted_data.tokens[i][0]=='0')
					return BAD_PLAYER_INDEX;
				if (splitted_data.tokens[i][1] != '\0')
					return BAD_PLAYER_INDEX;
			}
			if (j % 3 == 2)
			{
				c = splitted_data.tokens[i][0];
				if (c == '0')
					return BAD_PLAYER_SCORE;
				for (p = splitted_data.tokens[i]; *p != '\0'; p++)
					if (!isdigit(*p))
						return BAD_PLAYER_SCORE;
			}
		}
	}
	int players_count = (j + 1) / 3;

	if (2 + height * width + players_count * 3 != splitted_data.size)
		return BAD_FILE;
	return ALL_COOL;
}

//Vladimir Seniv
ERRORS checkMap(const char* filename)
{
	FILE* file = NULL;
	file = fopen(filename, "r"); // read
	if (file == NULL)return BAD_FILE; //file can't be opened
	int file_size = fileSize(file);
	char* all_data = (char*)malloc((file_size + 1) * sizeof(char));
	fread(all_data, sizeof(char), file_size, file);
	all_data[file_size] = '\0';
	fclose(file);
	TFormattedData temp = split_data(all_data);
	int result = check_formatted(temp);
	free(temp.tokens);
	free(all_data);
	return result;
}

//Dawid Badurek
int readMap(char* fileName, struct GameData* data, struct Field*** map,struct Player* ourPlayer)
{
	//if any error occured printf proper message and return result code as in .pdf file
	switch (checkMap(fileName))
	{
	case BAD_FILE:
		printf("EROOR: File has unproper format or can't bo opened.\n");
		return 2;//error code from the .pdf file
	case BAD_DIMENSIONS:
		printf("ERROR: The dimensions of board is unproper.\n");
		return 2;
	case BAD_CELL: 
		printf("ERROR: There is unproper value on the one of the fiels.\n");
		return 2;
	case BAD_PLAYER_NAME:
		printf("ERROR: Unproper player's name.\n");
		return 2;
	case BAD_PLAYER_INDEX:
		printf("ERROR: Unproper player's id.\n");
		return 2;
	case BAD_PLAYER_SCORE:
		printf("ERROR: Unproper player's score.\n");
		return 2;
	case BAD_PENGUINS:
		printf("ERROR: There is unproper number of penguins on the board.\n");
		return 2;
	default:
		//everything is ok
		break;
	}

	FILE* file;
	file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("ERROR: File %s doesn't exist or couldn't be opened.\n",fileName);
		return 2;
	}
	char input[30];
	//dimensons of the board
	for (int x = 0; x < 2; x++)
	{
		fscanf(file, "%s", input);

			if(x==0)
			data->y = atoi(input);
			else
			data->x = atoi(input);
	}

	//allocating memory for the board
	*map = (struct Field**) malloc(data->x * sizeof(struct Field*));
	if (*map == NULL)
	{
		printf("Error. The program couldn't allocate memory.");
		return 1;
	}
	for (int i = 0; i < data->x; i++)
	{
		(*map)[i] = (struct Field*) malloc(data->y * sizeof(struct Field));
		if ((*map)[i] == NULL)
		{
			printf("Error. The program couldn't allocate memory.");
			return 1;
		}
	}

	//reads board
	for (int y = 0; y < data->y; y++)
	{
		for (int x = 0; x < data->x; x++)
		{
			fscanf(file, "%s", input);
			(*map)[x][y].fish = input[0]-'0';
			(*map)[x][y].penguinID = input[1]-'0';
		}
	}

	//reading players
	struct List players;
	players.next = NULL;
	struct List* current = &players;
	int counter = 0;
	//check if we are on the list
	int areWeOnList = 0;
	while (1)
	{
		
		fscanf(file, "%s", input);
		if (feof(file) == 1)break;
		strcpy(current->player.id, input);
		counter++;

		fscanf(file, "%s", input);
		current->player.number = atoi(input);

		fscanf(file, "%s", input);
		current->player.result = atoi(input);

		if (strcmp(current->player.id, ID) == 0)
		{
			areWeOnList = 1;
			ourPlayer->number = current->player.number;
			ourPlayer->result = current->player.result;
		}

		//move list down
		current->next = (struct List*)malloc(sizeof (struct List));
		current->next->next = NULL;
		current = current->next;
	}

	data->players = counter;
	if (areWeOnList == 0)data->players++;
	data->players_data = (struct Player*)malloc(data->players * sizeof(struct Player));

	//adds players to data
	current = &players;
	for (int x = 0; x < counter; x++)
	{
		strcpy(data->players_data[x].id, current->player.id);
		data->players_data[x].number = current->player.number;
		data->players_data[x].result = current->player.result;
		current = current->next;
	}
	//clear list
	freeList(&players);

	//if we aren't on the list we will be added
	if (areWeOnList == 0)
	{
		int isUsed = 0;
		for (int x = 1; x < 10; x++)
		{
			isUsed = 0;
			for(int y = 0; y < counter; y++)
			{
				if (data->players_data[y].number == x)
				{
					isUsed = 1;
					break;
				}
			}
			if (isUsed == 0)
			{
				ourPlayer->number = x;
				strcpy(data->players_data[data->players-1].id, ourPlayer->id);
				data->players_data[data->players-1].number = ourPlayer->number;
				data->players_data[data->players-1].result = ourPlayer->result;
				break;
			}
		}
		
	}
	return 0;
}

//changes int value to string
//Dawid Badurek
char* itos(int a)
{
	int size = 1;
	while (a >= pow(10, size)) { size++; }
	char* buffer = (char*)malloc((size+1) * sizeof(char));

	for(int i=size-1;i>=0;i--)
	{
		buffer[i] = a % 10+'0';
		a/= 10;
	}
	buffer[size] = '\0';
	return buffer;
}
//Dawid Badurek
int isTXT(char* file)
{
	return strcmp(file + strlen(file) - 4, ".txt") == 0;
}
//Dawid Badurek
void freeList(struct List* list)
{
	if (list->next == NULL)return;
	struct List* p1 = list->next;
	struct List* p2 = list->next;
	while (p1->next != NULL)
	{
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}
	free(p1);
	list->next = NULL;
}

//Piotr Kitlowski
int isBlocked(int x, int y, struct GameData* data, struct Field** map)
{
	//LEFT
	if (x != 0)
	{
		if (map[x - 1][y].fish != 0)
			return 0;//left field isn't melted
	}
	//RIGHT
	if (x != (data->x - 1))
	{
		if (map[x + 1][y].fish != 0)
			return 0;//right field isn't melted
	}
	//UP
	if (y != 0)
	{
		if (map[x][y - 1].fish != 0)
			return 0;//upper field isn't melted
	}
	//DOWN
	if (y != (data->y - 1))
	{
		if (map[x][y + 1].fish != 0)
			return 0;//down field isn't melted
	}

	return 1;

}

//looks if specific player can move his/her penguins
//Piotr Kitlowski
int isMovePossible(int Player, struct GameData* data, struct Field** map)
{

	for (int x = 0; x < data->x; x++)
	{
		for (int y = 0; y < data->y; y++)
		{
			if (map[x][y].penguinID == Player)
			{
				if (isBlocked(x, y, data, map) == 0)return 1;//return value true if the move is possible
			}
		}
	}
	return 0;//returns value false if the move is not possible
}

//A function which shows if the game ends
//Piotr Kitlowski
int isGameEnd(struct GameData* data, struct Field** map)
{
	for (int x = 1; x <= data->players; x++)
	{
		if (isMovePossible(x, data, map) == 1)
		{
			return 0;//return false value when it is possible to move a penguin
		}
	}
	return 1;//return true value when the game can't go on any longer
}
