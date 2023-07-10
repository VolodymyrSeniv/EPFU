#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include "common.h"

//asks players for game data like penguins, board size etc. - Dawid Badurek
int gameMenu(struct GameData*,struct Field***);
//shows entered data, just for testing - Dawid Badurek
void info(struct GameData*);


//check whether it is possible to move to choosen field - Piotr Kitlowski
int isRoad(int a, int b, int i, int j, struct Field** map);
/*Each turn, the player will have to make a move(if she/he can).
Need to give specific point on the map(x,y).
Piotr Kitlowski */
void movement(struct GameData*, struct Field**);

//prints the score board - Oliwia Pęczkowska
void printResult(struct GameData* data);
/*Places the penguin on the board for player. Player will use console to type the coordinates of field to place the penguin on.- Oliwia Peczkowska <3
  ID - the ID of player which will place the penguin
  return int - return 0(error) or 1 depends if placement were succeded*/
void placement(struct GameData*, struct Field**);

//Generates map and saves it into global data structure - Dawid Badurek
void generateMap(struct GameData*, struct Field**);

// Draws the map in the console using printf - Dawid Badurek
void drawMap(struct GameData*, struct Field**);

//change the size of console to fit the board - Dawid Badurek
void resizeConsole(struct GameData* data);



#endif// INTERACTIVE_H
