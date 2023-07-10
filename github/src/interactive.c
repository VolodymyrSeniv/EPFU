#define _CRT_SECURE_NO_WARNINGS //just in visual studio



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "interactive.h"



//Dawid Badurek
int gameMenu(struct GameData* data,struct Field*** map)
{
	//to store input
	char input1[5];
	char input2[5];

	//number of players
	while (1)
	{
		printf("Enter a number of players(number from 1 to 9)\n");

		scanf("%4s", input1);
		//clears the buffor
		while (getchar() != '\n');
		

		data->players = atoi(input1);

		if (data->players< 1 || data->players>9)
		{
			printf("Error. You have entered unproper value of players.\n");
			continue;
		}
		break;
	}

	data->players_data = (struct Player*)malloc(data->players * sizeof(struct Player));

	//number of penguins
	while (1)
	{
		printf("Enter a number of penguins(number from 1 to 9:\n");

		scanf("%4s", input1);
		//clears the buffor
		while (getchar() != '\n');

		data->penguins = atoi(input1);


		if (data->penguins < 1 || data->penguins>9)
		{
			printf("Error. You have entered unproper value of penguins.\n");
			continue;
		}
		break;
	}

	//board size
	while (1)
	{
		printf("Enter a number of rows (number from 5 to 100)\n");
		scanf("%4s", input1);
		//clears the buffor
		while (getchar() != '\n');

		printf("Enter a number of columns (number from 5 to 100)\n");
		scanf("%4s", input2);
		//clears the buffor
		while (getchar() != '\n');

		data->y = atoi(input1);
		data->x = atoi(input2);

		if (data->x < 5 || data->x>100 || data->y < 5 || data->y>100)
		{
			printf("Error. You have entered unproper value.\n");
			continue;
		}

		if ((data->players * data->penguins) > (data->x * data->y))
		{
			printf("Error. This field would be too small for that amount of penguins.\n");
			continue;
		}
		break;
	}

	//allocate memory for map
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

	//player's names
	for (int i = 0; i < data->players; i++)
	{
		printf("Enter the username of player %i (max 25 characters)\n", i + 1);
		scanf("%25s", data->players_data[i].id);
		data->players_data[i].number = i+1;
		data->players_data[i].result = 0;
		//clears the buffor
		while (getchar() != '\n');
	}

}



//shows the data entered by user
//Dawid Badurek
void info(struct GameData* data)
{
	printf("Players: %i\nPenguins: %i\nX: %i\nY: %i\n", data->players, data->penguins, data->x, data->y);
	for (int z = 0; z < data->players; z++) printf("Player %i: %s\n", z + 1, data->players_data[z].id);
}


/*Generate map and put it into global data structure - Dawid Badurek*/
void generateMap(struct GameData* data,struct Field** map)
{
	//to hold random value
	int random = 0;

	for (int i = 0; i < data->x; i++)
	{
		for (int j = 0; j < data->y; j++)
		{
			map[i][j].fish = 0;
			map[i][j].penguinID = 0;
		}
	}

	//creates enough fields with 1 fish at first
	int counter = data->players * data->penguins;
	int x, y;
	while (counter > 0)
	{
		x = rand() % data->x;
		y = rand() % data->y;
		if (map[x][y].fish == 0)
		{
			map[x][y].fish = 1;
			counter--;
		}	
	}

	for (int i = 0; i < data->x; i++)
	{
		for (int j = 0; j < data->y;j++)
		{
			//fill the map with fish or empty places
			if (map[i][j].fish == 1)continue;
			random = rand() % 100;
			if (random >= 85)map[i][j].fish = 3;
			else if (random >= 40)map[i][j].fish = 2;
			else if (random >= 15)map[i][j].fish = 1;
			else map[i][j].fish = 0;
		}
	}
	
	
}

/* Draws the map in the console using printf - Dawid Badurek */
void drawMap(struct GameData* data, struct Field** map)
{
	printf("       ");
	//number of columns
	for (int i = 0; i < data->x; i++)
	{
		if(i+1<10)printf("%d   ", i+1);
		else
		printf("%d  ",i+1);
	}
	printf("\n\n\n");

	for (int i = 0; i < data->y; i++)
	{
		//number of rows
		if(i+1<10)printf("%d      ", i + 1);
		else printf("%d     ", i+1);

		for (int j = 0; j < data->x; j++)
		{
			printf("%i%i  ", map[j][i].fish, map[j][i].penguinID);
		}
		printf("\n");
	}
}

//change the size of console to fit the board - Dawid Badurek
void resizeConsole(struct GameData* data)
{
	int width = 6 + 4 * data->x;
	int height = 25 + 2 * data->y;
#ifdef WINDOWS
	char str[50] = "mode con: cols=\0";
	strcpy(str + strlen(str), itos(width));
	strcpy(str + strlen(str), " lines=");
	strcpy(str + strlen(str), itos(height));
	system(str);
#endif
#ifdef LINUX
	char str[50] ="\e[8;";
	strcpy(str + strlen(str),itos(height));
	strcpy(str + strlen(str), ";");
	strcpy(str + strlen(str),itos(width));
	strcpy(str + strlen(str), "t");
	printf("%s",str);
#endif
#ifdef MAC
	char str[50] = "\e[8;";
	strcpy(str + strlen(str), itos(height));
	strcpy(str + strlen(str), ";");
	strcpy(str + strlen(str), itos(width));
	strcpy(str + strlen(str), "t");
	printf("%s", str);
#endif

}

//Oliwia Peczkowska
void printResult(struct GameData* data){
    
    for (int z = 0; z < data->players; z++)
        {
            printf("Player %i: %s points %d \n", z + 1, data->players_data[z].id, data->players_data[z].result);
        }
    printf("\n");
    
    
}

//Oliwia Peczkowska
void placement(struct GameData* data, struct Field** map)
{
    
        printf("\n");
    
        printResult(data);
        
        printf("\n");
        
        for(int m = 0; m < data->penguins; m++ )
        {
            
            for(int n = 0; n < data->players; n++)
            {
				while (1)
				{
					int y_place, x_place;
					char y_change[5];
					char x_change[5];
					printf("Give me a place for penguin %d for Player %d for columns and rows:\n", m+1, n+1);
					scanf("%4s %4s", y_change, x_change);

					while (getchar() != '\n');


					y_place = atoi(y_change) - 1;
					x_place = atoi(x_change) - 1;

					if (x_place < 0 || x_place > data->y - 1 || y_place < 0 || y_place > data->x - 1)
					{
						printf("Unproper value\n");
						continue;
					}

					if (map[y_place][x_place].fish == 1 && map[y_place][x_place].penguinID == 0)
					{
						data->players_data[n].result += map[y_place][x_place].fish;
						map[y_place][x_place].penguinID = n+1;
						map[y_place][x_place].fish = 0;
						break;
					}
					else if (map[y_place][x_place].fish == 2 || map[y_place][x_place].fish == 3)
					{
						printf("Choose place with one fish\n");
						printf("\n");
						continue;
					}
					else
					{
						printf("This place occupied or melted\n");
						printf("\n");
						continue;
					}
				}

				clearConsole();

                drawMap(data,map);
                
                printf("\n");
               
                printResult(data);
                
                printf("\n");

			 }
		}
}

//Piotr Kitlowski
void movement(struct GameData* data, struct Field** map)
{
	int a, b;
	int i, j;	
	int currentPlayer=0;
			
	while(1)
	{		
	
    		if(isGameEnd(data,map)==1)
    		{
    			break;
    		}
    		
		do
		{
			if(currentPlayer<data->players)
			{
				currentPlayer++;
			}
    			else currentPlayer=1;
         
		}while(isMovePossible(currentPlayer, data, map)==0);
        
    
		while(1)
		{
			printf("Give coordinates of penguin you want to move\n");
			scanf("%d %d", &a, &b);
			
			while (getchar() != '\n');
			
			a--;
			b--;

          	if(a<0 || a>data->x || b<0 || b>data->y)
			{
				printf("This field doesn't exist\n");
				continue;
			}
          
			if(map[a][b].penguinID!=currentPlayer)
			{
				printf("You have choosen not your penguin\n");
				printf("It is player %d turn\n", currentPlayer);
				continue;
			}


			if(isBlocked(a,b,data,map)==1)
			{

				printf("You can not choose this pepnguin since it's blocked\n");
				continue;

			}
			break;
		}
    
    
          while(1)
          {
			printf("Where would you like to move penguin from %d %d\n",a+1,b+1);
			scanf("%d %d", &i, &j);	

			while (getchar() != '\n');

			i--;
			j--;

			if(i<0 || i>=data->x || j<0 || j>=data->y)
			{
				printf("This field doesn't exist");
				continue;
			}

			if(map[i][j].fish==0)
			{
				printf("Field is melted\n");
				continue;
			}

			if(a!=i && b!=j)
			{
				printf("You can move penguin only on straight lines\n");
				continue;
			}
			//
			if(isRoad(a, b, i, j, map)==0)
			{
				printf("You can't move to that field, melted filed or there is already a penguin\n");
				continue;
			}

			//moves penguin to specific place after checking all 
			map[i][j].penguinID=currentPlayer;
			data->players_data[currentPlayer-1].result+=map[i][j].fish;
			map[i][j].fish=0;
			map[a][b].penguinID=0;


			clearConsole();
			drawMap(data, map);
			printResult(data);
			break;
			
		}
	}
	
	printf("The game has ended\n");//End of the game and showed scores
	clearConsole();
	printResult(data);
	
}

//Piotr Kitlowski
int isRoad(int a, int b, int i, int j, struct Field** map)
{
	//move y line |||||||||||||
	//			||||||||||||
	//			||||||||||||
	if(a==i)
	{
	
		if(b>j)
		{
			for(int y = b-1; y>j; y--)
			{
				if(map[a][y].fish==0)
					return 0;//no road
			}
		}
		else
		{
			for(int y = b+1; y<j; y++)
			{
				if(map[a][y].fish==0)
					return 0;//no road
			}
		}
	}
	//move x line --------------
	//			-------------	
	//			-------------
	else
	{

		if(a>i)
		{
			for(int x = a-1; x>i; x--)
			{
				if(map[x][b].fish==0)
				return 0;//no road
			}
		}
		else
		{
			for(int x = a+1; x<i; x++)
			{
				if(map[x][b].fish==0)
				return 0;//no road
			}
		}

	}

	return 1;//If there is unblocked road the funtions return true value

}




