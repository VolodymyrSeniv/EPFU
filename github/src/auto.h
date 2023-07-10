
/////////////////////////////////////////////////////////////////////////
////////auto.h is coded by Dawid Badurek
////////////////////////////////////////////////////////////////////////

#ifndef AUTO_H
#define AUTO_H

//factors used in alghorithm, for every field factor will be calculated using these factors below
#define FISH_FACTOR 1
#define PENGUIN_FACTOR 2

//used in alghorythm, holds move data
struct Move
{
	int x_penguin;
	int y_penguin;
	int x_move;
	int y_move;
};

//tree data structure
struct Tree
{
	struct Move move;
	float factor;
	struct Tree* higher;
	struct Tree* lower;
	struct Tree* equal;
};
//functions to operate on structures
//add struct move to tree
void addToTree(struct Tree*, struct Move, float);
//find the tree with the highest factor and return it
struct Tree* findTheHighest(struct Tree* tree);
//return empty structure
struct Tree* createEmpty();
//clear all the pointers inside tree
void freeTree(struct Tree**);
//count the factor of the field - the factor tell us how good is this place(how many possibilities to move is from here)
//return the value of factor
float countFactor(int, int, struct GameData*, struct Field**, struct Player*);
//count factors for all possible moves and save it inside a tree
void countMoves(int, int, struct Tree*, struct GameData*, struct Field**, struct Player*);
//automatically moves penguins
//return result code as in .pdf file
int autoMovement(struct GameData*, struct Field**, struct Player*,char*);
//automatically places penguins
//return result code as in .pdf file
int autoPlacement(struct GameData*, struct Field**, struct Player*,char*);

#endif //AUTO_H