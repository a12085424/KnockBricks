#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define UPRIGHT1 1
#define UPRIGHT2 2
#define UPRIGHT3 3
#define DOWNRIGHT1 4
#define DOWNRIGHT2 5
#define DOWNRIGHT3 6
#define DOWNLEFT1 7
#define DOWNLEFT2 8
#define DOWNLEFT3 9
#define UPLEFT1 10
#define UPLEFT2 11
#define UPLEFT3 12
#define keyup(VK_CODE) ((GetAsyncKeyState(VK_CODE) & 0x0001) ?1:0) 

typedef struct Brick {   //×©¿é½á¹¹ÉùÃ÷
	int x;
	int y;
	struct Brick* next;
} Brick;

typedef struct Ball {
	int x;
	int y;
	int angle;
} Ball;

/*È«¾Ö±äÁ¿*/
Brick* first = NULL;  //µÚÒ»¿é×©
Brick* wall = NULL;
Ball* ball = NULL;
Brick* tray = NULL;
Brick* p = NULL;
Brick* q = NULL;




/*ÉùÃ÷ËùÓÐº¯Êý*/
void pos(int x, int y);
void initmap(void);
void initbricks(void);
void inittray(void);
void initball(void);
void traymove(void);
void ballmove(void);
Ball* ballbounce(Ball* next);
void brickbreak(Brick* brick);
void endgame(void);
void gameloop(void);
void welcometogame(void);

void pos(int x, int y) {
	COORD pos;
	HANDLE Houtput;
	pos.X = x;
	pos.Y = y;
	Houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(Houtput, pos);
}

void initmap(void) {
	Brick* nextwall = NULL;
	int i;
	wall = (Brick*)malloc(sizeof(Brick));
	wall->x = 0;
	wall->y = 30;
	wall->next = NULL;
	for (i = 29; i >= 0; i -= 1) {
		nextwall = (Brick*)malloc(sizeof(Brick));
		nextwall->x = 0;
		nextwall->y = i;
		nextwall->next = wall;
		wall = nextwall;
	}
	for (i = 2; i <= 56; i += 2) {
		nextwall = (Brick*)malloc(sizeof(Brick));
		nextwall->x = i;
		nextwall->y = 0;
		nextwall->next = wall;
		wall = nextwall;
	}
	for (i = 1; i <= 30; i += 1) {
		nextwall = (Brick*)malloc(sizeof(Brick));
		nextwall->x = 56;
		nextwall->y = i;
		nextwall->next = wall;
		wall = nextwall;
	}
	for (p = wall; p != NULL; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("¡õ");
	}

}

void initbricks(void) {
	Brick* brick = NULL;
	int i;
	first = (Brick*)malloc(sizeof(Brick));
	first->x = 2;
	first->y = 1;
	first->next = NULL;
	for (i = 4; i <= 54 * 4; i += 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = (i - 2) % 54 + 2;
		brick->y = (i - 2) / 54 + 1;
		brick->next = first;
		first = brick;
	}
	for (p = first; p != NULL; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("¡ö");
	}

}

void initbricks2(void) {
	Brick* brick = NULL;
	int i;
	first = (Brick*)malloc(sizeof(Brick));
	first->x = 2;
	first->y = 1;
	first->next = NULL;
	for (i = 4; i <= 54; i += 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = i;
		brick->y = 1;
		brick->next = first;
		first = brick;
	}
	for (i = 2; i <= 29; i += 1) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = 54;
		brick->y = i;
		brick->next = first;
		first = brick;
	}
	
	for (i = 28; i >= 2; i -= 1) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = 2;
		brick->y = i;
		brick->next = first;
		first = brick;
	}
	for (i = 16; i <= 40; i += 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = i;
		brick->y = 10;
		brick->next = first;
		first = brick;
	}
	for (i = 15; i <= 20; i += 1) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = 28;
		brick->y = i;
		brick->next = first;
		first = brick;
	}

	brick = (Brick*)malloc(sizeof(Brick));
	brick->x = 20;
	brick->y = 15;
	brick->next = first;
	first = brick;


	for (p = first; p != NULL; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("¡ö");
	}
}

void inittray(void) {
	Brick* nexttray;
	tray = (Brick*)malloc(sizeof(Brick));
	int i;
	tray->x = 24;
	tray->y = 28;
	tray->next = NULL;
	for (i = 26; i <= 32; i += 2) {
		nexttray = (Brick*)malloc(sizeof(Brick));
		nexttray->x = i;
		nexttray->y = 28;
		nexttray->next = tray;
		tray = nexttray;
	}
	for (p = tray; p != NULL; p = p->next) {
		pos(p->x, p->y);
		printf("¡ö");
	}
}

void initball(void) {
	ball = (Brick*)malloc(sizeof(Brick));
	ball->x = 4;
	ball->y = 10;
	ball->angle = 2;
	pos(ball->x, ball->y);
	printf("¡ñ");
}

void traymove(void) {
	if (keyup(VK_RIGHT) && tray->x < 54) {
		for (p = tray; p != NULL; p = p->next) {  //Çå³ýÔ­À´µÄÍÐÅÌ£¬²¢°ÑÎ»ÖÃÓÒÒÆÒ»¸ñ
			pos(p->x, p->y);
			printf("  ");
			p->x += 2;
		}

		for (p = tray; p != NULL; p = p->next) {  //´òÓ¡ÍÐÅÌ
			pos(p->x,p->y);
			printf("¡ö");
		}
	}
	if (keyup(VK_LEFT) && tray->x > 10) {
		for (p = tray; p != NULL; p = p->next) {  //Çå³ýÔ­À´µÄÍÐÅÌ£¬²¢°ÑÎ»ÖÃ×óÒÆÒ»¸ñ
			pos(p->x, p->y);
			printf("  ");
			p->x -= 2;
		}

		for (p = tray; p != NULL; p = p->next) {  //´òÓ¡ÍÐÅÌ
			pos(p->x, p->y);
			printf("¡ö");
		}
	}
}

Ball* ballbounce(Ball* nextball) {
	switch (ball->angle) {
	case 1:
		for (p = first; p != NULL; p = p->next) {
			if (p->x == ball->x && p->y == ball->y - 1) {
				nextball->x = ball->x + 1;
				nextball->y = ball->y;
				nextball->angle = 5;
			}
			else if (p->x == ball->x + 1 && p->y == ball->y - 1) {
				nextball->x = ball->x;
				nextball->y = ball->y - 2;
				nextball->angle = 12;
			}
			else if (p->x == ball->x + 1 && p->y == ball->y - 2) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y - 1;
				nextball->angle = 5;
			}
		}

	}
	

}


void ballmove(void) {
	Ball* nextball = NULL;
	nextball = (Ball*)malloc(sizeof(Ball));
	int block = 0;
	time_t t;
	switch (ball->angle) {
	case 1:
		//ÅÐ¶Ï×²×©
		nextball->x = ball->x + 2;
		nextball->y = ball->y - 2;
		nextball->angle = 1;
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 2)) {
				block += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 6;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 12;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y - 1;
			nextball->angle = 6;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 11;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 2)) {
				block += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 6;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 12;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y - 1;
			nextball->angle = 6;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 11;
		}

		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;

	case 2:
		nextball->x = ball->x + 2;
		nextball->y = ball->y - 1;
		nextball->angle = 2;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 11;
			break;
		case 101:
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 4;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y - 1;
				nextball->angle = rand() % 3 + 10;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 9 + 7;
			}
			break;
		default:
			break;

		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 11;
			break;
		case 101:
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 4;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y - 1;
				nextball->angle = rand() % 3 + 10;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 9 + 7;
			}
			break;
		default:
			break;

		}

		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 3:
		nextball->x = ball->x + 4;
		nextball->y = ball->y - 1;
		nextball->angle = 3;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y - 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 10000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 10;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y - 1;
			nextball->angle = 10;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 5;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y - 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 10000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 10;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y - 1;
			nextball->angle = 10;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 5;
		}

		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	

	case 4:
		nextball->x = ball->x + 4;
		nextball->y = ball->y + 1;
		nextball->angle = 4;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y + 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 3;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 9;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 2;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y + 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 3;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 9;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 2;
		}
		//ÅÐ¶Ï×²ÅÌ
		block = 0;
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y + 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (block % 100 == 10) {
			if (block / 1000 == 1) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y;
				nextball->angle = 3;
			}
			else {
				srand((unsigned)time(&t));
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 10;
				
			}
			
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 9;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = rand() % 2 + 3;
		}


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;






	case 5:
		nextball->x = ball->x + 2;
		nextball->y = ball->y + 1;
		nextball->angle = 5;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 101:
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 1;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y + 1;
				nextball->angle = rand() % 3 + 7;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 10;
			}
			break;
		default:
			break;

		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 101:
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 1;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y + 1;
				nextball->angle = rand() % 3 + 7;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 10;
			}
			break;
		default:
			break;

		}
		//ÅÐ¶Ï×²ÅÌ
		block = 0;
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 1:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 10:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 3 * (rand() % 2) + 2;
			break;
		case 100:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 9 * (rand() % 2) + 1;
			break;
		default:
			break;

		}


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;



	case 6:
		nextball->x = ball->x + 2;
		nextball->y = ball->y + 2;
		nextball->angle = 6;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 2)) {
				block += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 1;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 7;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 1;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 8;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 2)) {
				block += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 1;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 7;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 1;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 8;
		}
		//ÅÐ¶Ï×²ÅÌ
		block = 0;
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 2)) {
				block += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 1;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 7;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 1;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 3 * (rand() % 2) + 6;
		}
		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;

	case 7:
		nextball->x = ball->x - 2;
		nextball->y = ball->y + 2;
		nextball->angle = 7;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 2)) {
				block += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 12;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 6;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y + 1;
			nextball->angle = 12;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 5;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 2)) {
				block += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 12;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 6;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y + 1;
			nextball->angle = 12;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 5;
		}
		//ÅÐ¶Ï×²ÅÌ
		block = 0;
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 2)) {
				block += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 12;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 6;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 12;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 3 * (rand() % 2) + 4;
		}


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;



	case 8:
		nextball->x = ball->x - 2;
		nextball->y = ball->y + 1;
		nextball->angle = 8;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 101:
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 5;
			break;
		case 110:
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x - 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 10;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y + 1;
				nextball->angle = rand() % 3 + 4;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 9 + 1;
			}
			break;
		default:
			break;

		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 2;
			break;
		case 101:
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 5;
			break;
		case 110:
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x - 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 10;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y + 1;
				nextball->angle = rand() % 3 + 4;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 9 + 1;
			}
			break;
		default:
			break;

		}
		//ÅÐ¶Ï×²×©
		block = 0;
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 1:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 110:
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
			break;
		case 10:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 3 * (rand() % 2) + 8;
			break;
		case 100:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 9 * (rand() % 2) + 3;
			break;
		default:
			break;

		}


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 9:
		nextball->x = ball->x - 4;
		nextball->y = ball->y + 1;
		nextball->angle = 9;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y + 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 10;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y + 1;
			nextball->angle = 4;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y + 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 10;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y + 1;
			nextball->angle = 4;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
		}
		//ÅÐ¶Ï×²ÅÌ
		block = 0;
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y + 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (block % 100 == 10) {
			if (block / 1000 == 1) {
				nextball->x = ball->x - 2;
				nextball->y = ball->y;
				nextball->angle = 10;
			}
			else {
				srand((unsigned)time(&t));
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 1;

			}

		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 4;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = rand() % 2 + 9;
		}

		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;

	case 10:
		nextball->x = ball->x - 4;
		nextball->y = ball->y - 1;
		nextball->angle = 10;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y - 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 3;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y - 1;
			nextball->angle = 3;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y - 1)) {
				block += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 3;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y - 1;
			nextball->angle = 3;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
		}


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 11:
		nextball->x = ball->x - 2;
		nextball->y = ball->y - 1;
		nextball->angle = 11;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 101:
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 2;
			break;
		case 110:
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x - 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 7;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y - 1;
				nextball->angle = rand() % 3 + 1;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 9 + 4;
			}
			break;
		default:
			break;

		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 101:
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 2;
			break;
		case 110:
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = ball->x - 2;
				nextball->y = ball->y;
				nextball->angle = rand() % 3 + 7;
			}
			else if (rand() % 100 > 33) {
				nextball->x = ball->x;
				nextball->y = ball->y - 1;
				nextball->angle = rand() % 3 + 1;
			}
			else {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = rand() % 9 + 4;
			}
			break;
		default:
			break;

		}

		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;




	case 12:
		nextball->x = ball->x - 2;
		nextball->y = ball->y - 2;
		nextball->angle = 12;
		//ÅÐ¶Ï×²×©
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 2)) {
				block += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 7;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 1;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y - 1;
			nextball->angle = 7;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 2;
		}
		//ÅÐ¶Ï×²Ç½
		block = 0;
		for (p = wall; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				block += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 2)) {
				block += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				block += 1000;
			}
		}
		if (block % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 7;
		}
		else if (block % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 1;
		}
		else if (block % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y - 1;
			nextball->angle = 7;
		}
		else if (block % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 2;
		}


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;



	}
}





void gameloop(void) {
	int n = 0;
	while (ball != NULL) {
		if (ball->y > 35) {
			exit(0);
		}
		traymove();
		if (n % 80 == 0) {
			ballmove();
			n = 1;
		}
		pos(60, 29);
		Sleep(1);
		n++;
	}
	
	
}


int main(void) {
	//system("mode con cols=100 lines=30");
	initmap();
	initbricks();
	inittray();
	initball();
	gameloop();
}