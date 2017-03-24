/*
* Test.c
*
* Created: 17.03.2017 18:27:17
* Author : Lars
*/
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "LEDs.h"
#include "lcd.h"

#define Button1 0b01111111
#define Button2 0b10111111
#define BOOL uint8_t
#define TRUE 1
#define FALSE 0
#define RAND_MAX 1

#define WIDTH 2
#define HEIGHT 17


#define MAXENEMYS 6


typedef enum GameStates
{
	Start,
	GamePlay,
	End
};

uint8_t getIndex(uint8_t row, uint8_t col){
	return row*HEIGHT+col;
}

void clearField(char * field)
{
	for(uint8_t i =0; i<WIDTH;i++)
	{
		for(uint8_t j =0; j<HEIGHT;j++)
		{
			field[getIndex(i,j)] = ' ';
		}
	}
	
	field[getIndex(0,16)] = '\n';
	field[getIndex(1,16)] = '\0';
	
}

void spawnEnemy(uint8_t * Liste[MAXENEMYS])
{
	uint8_t freeSpace = MAXENEMYS + 1;
	uint8_t distance = 100;
	for(uint8_t i = 0; i<MAXENEMYS;i++)
	{
		if(Liste[i]==NULL)
		{
			freeSpace = i;
		}
		else
		{
			if(15-Liste[i][1]<distance)
			{
				distance = 15-Liste[i][1];
			}
		}
	}
	
	if((distance>2 + (rand()&3) && freeSpace < MAXENEMYS)||distance == 100)
	{
		uint8_t * newEnemy = malloc(sizeof(uint8_t) * 3);
		newEnemy[0] = (uint8_t) (rand()%2);
		newEnemy[1] = 15;
		newEnemy[2] = 'Z';
		Liste[freeSpace] = newEnemy;
	}
}

void Update(uint8_t * List[MAXENEMYS],uint8_t * Health,uint8_t Player,enum GameStates * currentState)
{
	for(uint8_t i = 0; i<MAXENEMYS; i++)
	{
		if(List[i] != NULL)
		{
			
			List[i][1]--;
			
			if(List[i][1]==0)
			{
				if(List[i][0] == (Player))
				{
					(*Health)--;
					if((*Health) == 0)
					{
						for(int i = 0; i<MAXENEMYS; i++)
						{
							free(List[i]);
							List[i] = NULL;
							(*currentState) = End;
							return;
						}
					}
				}
				free(List[i]);
				List[i] = NULL;
			}
		}
	}
}

void GameLoop(enum GameStates * currentState)
{
	uint8_t health = 5;
	
	char * screen = malloc(WIDTH*HEIGHT*sizeof(char));
	uint8_t Player[3] = {1,0,'>'};
	uint8_t * myList[MAXENEMYS] ;

	for(uint8_t i = 0; i<MAXENEMYS;i++)
	{
		myList[i] = NULL;
	}
	

	uint8_t animationDelay = 0;
	while (1)
	{
		clearField(screen);
		
		
		if(PIND == Button1)
		{
			if(Player[0] == 1)
			{
				Player[0] = 0;
			}
			
		}
		if(PIND == Button2)
		{
			if(Player[0] == 0)
			{
				Player[0] = 1;
			}
		}
		
		if(animationDelay%(3 + (rand()%2)) == 0)
		{
			Update(myList,&health,Player[0],currentState);
			spawnEnemy(myList);
		}

		screen[getIndex(Player[0],Player[1])] = (char) Player[2];
		
		for(uint8_t i = 0; i<MAXENEMYS; i++)
		{
			if(myList[i] != NULL)
			screen[getIndex(myList[i][0], myList[i][1])] = (char) myList[i][2];
		}
		
		
		
		lcd_clrscr();
		lcd_puts(screen);
		ShowHealth(health);
		_delay_ms(1000);
		animationDelay++;
		if(health == 0)
		{
			return;
		}
	}
}

void StartScreen(enum GameStates * currentState)
{
	lcd_clrscr();
	lcd_puts("Press a Key \nTo PLAY");
	
	while(1)
	{
		if(PIND == Button1 || PIND == Button2)
		{
			(*currentState) = GamePlay;
			return;
		}
	}
}

void EndScreen(enum GameStates * currentState)
{
	lcd_clrscr();
	lcd_puts("Game Over");
	while(1)
	{
		if(PIND == Button1 || PIND == Button2)
		{
			(*currentState) = Start;
			return;
		}
	}
}
uint8_t main()
{
	
	
	DDRD = 0x00;
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	enum GameStates gameState = Start;
	srand(time(NULL));
	while(1)
	{
	 switch(gameState)
	{
		case Start:
		StartScreen(&gameState);
		break;
		case GamePlay:
		GameLoop(&gameState);
		break;
		case End:
		EndScreen(&gameState);
		break;
		default:
		lcd_clrscr();
		lcd_puts("Unreachable code");
		break;
	}
	}

}

