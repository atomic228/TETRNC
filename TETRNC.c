#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>

void gotoxy(int xpos, int ypos);

#define KEY_UP 72 
#define KEY_DOWN 80 
#define KEY_LEFT 75 
#define KEY_RIGHT 77 
#define KEY_ESC 27 
#define KEY_ENTER 13 

#define SIZEX 16
#define SIZEY 18
#define FMAP_COUNTS 7 

#define TRUE 1
#define FALSE 0

#define SCR_SP  '\xB0' 
#define SCR_OB  '\xFE' 

const char* GAME_TITLE = "###########  ###########   ###########     #########  #####    ###      ##### #\n"
"#    #    #    #       #   #    #    #    #      #     #      # #    #        #\n"
"#    #    #    #   #       #    #    #    #      #     #     #  #   ##         \n"
"     #         #####            #           ######     #   ##   #   #          \n"
"     #         #   #            #          #     #     #  #     #    #         \n"
"     #         #       #        #         #      #     # #      #     #       #\n"
"   #######    ###########     #######  ###    #####   ###    #####       ##### \n\n";

int screen[SIZEX][SIZEY] = { 0 };
int map[4][4];
int px, py, score, nextmap;

int fmap[FMAP_COUNTS][4][4] =
{
	{
		{ 1, 1, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 0, 0, 0 }
	},
	{
		{ 0, 0, 1, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 1, 1, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 1, 1, 1, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	}
};

void print(void)
{
	int i, j;
	int buff[SIZEX][SIZEY];

	for (i = 0; i < SIZEY; i++) for (j = 0; j < SIZEX; j++) buff[j][i] = screen[j][i];
	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) if (map[j][i]) buff[j + px][i + py] = 1;

	gotoxy(0, 0);
	for (i = 0; i < SIZEY; i++)
	{
		for (j = 0; j < SIZEX; j++)
		{
			putchar(buff[j][i] == 0 ? SCR_SP : SCR_OB);
		}
		putchar('\n');
	}

	gotoxy(SIZEX + 1, 0);
	printf("Score: %i", score);
}
void printnextmap(void)
{
	int i, j;

	gotoxy(SIZEX + 1, 2);
	printf("Next: ");

	for (i = 0; i < 4; i++)
	{
		gotoxy(SIZEX + 2, i + 3);
		for (j = 0; j < 4; j++)
		{
			putchar(fmap[nextmap][j][i] == 0 ? ' ' : SCR_OB);
		}
	}
}

int getkey(void)
{
	int c;
	if (_kbhit())
	{
		if ((c = _getch()) == 224) c = _getch();
		return c;
	}
	return FALSE;
}

void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

int valnewpos(int x, int y)
{
	int i, j;
	if (x < 0) return FALSE;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (map[j][i])
			{
				if ((j + x >= SIZEX) || (i + y >= SIZEY)) return FALSE;
				if (screen[j + x][i + y])
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}


#define inv(x) ((x * (-1)) + 3)



int rnd(int max)
{
	max++;
	return (int)(rand() * max / RAND_MAX);
}

void sleep(int milsec)
{
	clock_t t = clock();
	while (clock() - t < milsec);
}



void createmap(void)
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			map[j][i] = fmap[nextmap][j][i];
	py = 0;
	px = SIZEX / 2;

	nextmap = rnd(FMAP_COUNTS - 1);
	printnextmap();
}

void startgame(void)
{
	int i, j, c;
	time_t tm;

	system("cls");
	px = SIZEX / 2;
	py = 0;
	score = 0;

	tm = clock();

	nextmap = rnd(FMAP_COUNTS - 1);
	createmap();

	while (TRUE)
	{
		c = getkey();

		switch (c)
		{
			break;
		case KEY_DOWN:
			for (; valnewpos(px, py + 1); py++);
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					if (map[j][i]) screen[px + j][py + i] = 1;

			print();
			createmap();
			break;
		case KEY_LEFT:
			if (valnewpos(px - 1, py)) px--;
			break;
		case KEY_RIGHT:
			if (valnewpos(px + 1, py)) px++;
			break;
		case 'p':
			_getch();
			break;
		case KEY_ESC:
			return;
		}

		if ((clock() - tm) > 320)
		{
			tm = clock();

			if (!(valnewpos(px, py + 1)))
			{
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						if (map[j][i]) screen[px + j][py + i] = 1;

				createmap();
			}
			else py++;
		}

		print();

		for (i = 0; i < SIZEX; i++)
		{
			if (screen[i][0])
			{
				system("cls");
				gotoxy(2, 8);
				printf("Game Over :(");
				return;
			}
		}
	}
}

void gamemenu(void)
{
	int p = 1, c = 0;
	const char* GAME_MENU = "\t" "\x1A" "1. Start\n"
		"\t 2. Info\n"
		"\t 3. Exit";
	system("cls"); printf("%s%s", GAME_TITLE, GAME_MENU);
	while (TRUE)
	{
		c = _getch();

		gotoxy(8, 8 + p - 1);
		putchar(' ');

		switch (c)
		{
		case '1':
		case '2':
		case '3':
			p = c - '0';
		case KEY_ENTER:
			switch (p)
			{
			case 1:
				startgame();
				gotoxy(0, SIZEY); printf("Press ESC to exit to main menu...\n"); while (_getch() != KEY_ESC);
				break;
			case 2:
				system("cls");
				printf("%s%s", GAME_TITLE,
					"\t\x1B       Left \n"
					"\t\x1A       Right\n"
					"\t\x19       Put Down\n"
					"\tESC     Exit\n\n"

					"Press any key...\n");
				_getch(); if (_kbhit()) _getch();
				break;
			case 3: return;
			}
			system("cls"); printf("%s%s", GAME_TITLE, GAME_MENU);
			p = 1;
			break;
		case KEY_UP:
			if (p > 1) p--;
			break;
		case KEY_DOWN:
			if (p < 4) p++;
			break;
		case KEY_ESC:
			return;
		}
		gotoxy(8, 8 + p - 1);
		putchar('\x1A');
	}
}

void main(void)
{
	printf("%s%s", GAME_TITLE,
		"Press any key to begin...\n");
	_getch(); if (_kbhit()) _getch();

	gamemenu();
}