#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
//INTERACTIVE, AUTO depends which mode would you like to compile
#define AUTO_MODE

//WINDOWS, LINUX, MAC depends on the system you are compiling on(needed to be set only in interactive
#define WINDOWS

//id of our team
#define ID "Penguinators"

//for readMap() - Vladimir Seniv
typedef enum ERRORS {
	ALL_COOL = 0,
	BAD_FILE = -1,
	BAD_DIMENSIONS = -2,
	BAD_FIELD_DIMENSIONS = -3,
	BAD_CELL = -4,
	BAD_PLAYER_LIST = -5,
	BAD_PLAYER_NAME = -6,
	BAD_PLAYER_INDEX = -7,
	BAD_PLAYER_SCORE = -8,
	BAD_PENGUINS = -9
} ERRORS;
//for readMap() - Vladimir Seniv
typedef struct TFormattedData
{
	int size;
	char** tokens;
} TFormattedData;
//board is an array of struct Field
struct Field
{
	int fish;
	int penguinID;
};

struct Player
{
	char id[26];
	int number;
	int result;
};
//list data structure
struct List
{
	struct Player player;
	struct List* next;
};
struct GameData
{
	int x;
	int y;
	int players;
	int penguins;
	struct Player* players_data;
};

//reads the map from an input file
//return result code as in .pdf file Dawid Badurek
int readMap(char*, struct GameData*, struct Field***,struct Player*);

//used in functions used in checkMap() - Vladimir Seniv
int fileSize(FILE* file);
TFormattedData split_data(char* raw_data);
ERRORS check_formatted(TFormattedData splitted_data);
//check if the file is proper if no returns error code - Vladimir Seniv
ERRORS checkMap(const char* filename);

/*Creates an external .txt file, containing current state of the map, and each player's data: id, number, and result - Ania Krawczuk */
int createOutputFile(struct GameData*,struct Field**,char* name);

//functions to controle game state - Piotr Kitlowski
//Checks if the penguin is blocked
int isBlocked(int x, int y, struct GameData* data, struct Field** map);
//Checks if specific player can move
int isMovePossible(int Player, struct GameData* data, struct Field** map);
//Checks if the game ends (every penguin is blocked)
int isGameEnd(struct GameData* data, struct Field** map);


//changes int value to string - Dawid Badurek
char* itos(int);
//check if the file name(string) has .txt at the end - Dawid Badurek
int isTXT(char*);
//clear the console - Dawid Badurek
void clearConsole();
//clears list, use free() on the every pointer  - Dawid Badurek
void freeList(struct List* list);
#endif //MAIN_H
