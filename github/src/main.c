
/////////////////////////////////////////////////////////////////////////
////////main.c is coded by Dawid Badurek
////////////////////////////////////////////////////////////////////////



#define _CRT_SECURE_NO_WARNINGS //just in visual studio

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "interactive.h"
#include "auto.h"





int main(int argc, char* argv[])
{
	struct Field** map;
	struct GameData data;

#ifdef INTERACTIVE_MODE

	gameMenu(&data,&map);

	//dynamically creates the Map

	resizeConsole(&data);
	generateMap(&data,map);
	clearConsole(); 
	drawMap(&data,map);
	placement(&data, map);

	 movement(&data, map);

	getchar();

#endif //INTERACTIVE

#ifndef INTERACTIVE_MODE
#ifdef AUTO_MODE
	
	struct Player ourPlayer;
	strcpy(ourPlayer.id, ID);
	ourPlayer.number = 0;//will  be readen during redMap() function from file or generated
	ourPlayer.result = 0;

	if (argc == 1)
	{
		printf("Too few arguments.");
		return 3;
	}
	if (strcmp(argv[1], "phase=placement")==0)
	{
		if (argc != 5)
		{
			printf("Wrong number of arguments.");
			return 3;
		}
		
		if (strncmp(argv[2], "penguins=", 9 * sizeof(char))!=0)
		{
			printf("Unproper argument: %s", argv[2]);
			return 3;
		}
		
		data.penguins = atoi(argv[2] + 9);
		char* inputFile = argv[3];
		char* outputFile = argv[4];

		if (!isTXT(inputFile) || !isTXT(outputFile))
		{
			printf("Unproper text file name(lack of .txt).");
			return 3;
		}
		
		if (readMap(inputFile, &data, &map,&ourPlayer)==2)
			return 2;

		return autoPlacement(&data, map, &ourPlayer, outputFile);

	}
	else if (strcmp(argv[1], "phase=movement")==0)
	{
		if (argc != 4)
		{
			printf("Wrong number of arguments.");
			return 3;
		}

		char* inputFile = argv[2];
		char* outputFile = argv[3];

		if (!isTXT(inputFile) || !isTXT(outputFile))
		{
			printf("Unproper text file name(lack of .txt).");
			return 3;
		}

		if (readMap(inputFile, &data,&map,&ourPlayer) == 2)
			return 2;

		return autoMovement(&data,map,&ourPlayer,outputFile);

	}
	else if (strcmp(argv[1], "id")==0)
	{
		printf(ID);
	}
	else
	{
		printf("Unproper argument: %s", argv[1]);
		return 3;
	}
#endif //AUTO_MODE
#endif //INTERACTIVE_MODE
	return 0;
}


