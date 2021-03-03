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

typedef struct Brick {   //砖块结构声明
	int x;
	int y;
	struct Brick* next;
} Brick;

typedef struct Ball {
	int x;
	int y;
	int angle;
} Ball;

/*全局变量*/
Brick* first = NULL;  //第一块砖
Ball* ball = NULL;
Brick* tray = NULL;
Brick* p = NULL;
Brick* q = NULL;




/*声明所有函数*/
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
	int i;
	for (i = 0; i <= 56; i += 2) {
		pos(i, 0);
		printf("□");
	}
	for (i = 1; i <= 29; i++) {
		pos(0, i);
		printf("□");
		pos(56, i);
		printf("□");
	}
}

void initbricks(void) {
	Brick* brick = NULL;
	int i;
	first = (Brick*)malloc(sizeof(Brick));
	first->x = 2;
	first->y = 21;
	first->next = NULL;
	for (i = 4; i <= 54 * 4; i += 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = (i - 2) % 54 + 2;
		brick->y = (i - 2) / 54 + 21;
		brick->next = first;
		first = brick;
	}
	for (p = first; p != NULL; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("■");
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
	for (i = 52; i >= 2; i -= 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = i;
		brick->y = 29;
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
	for (p = first; p != NULL; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("■");
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
		printf("■");
	}
}

void initball(void) {
	ball = (Brick*)malloc(sizeof(Brick));
	ball->x = 4;
	ball->y = 10;
	ball->angle = 2;
	pos(ball->x, ball->y);
	printf("●");
}

void traymove(void) {
	if (keyup(VK_RIGHT) && tray->x < 54) {
		for (p = tray; p != NULL; p = p->next) {  //清除原来的托盘，并把位置右移一格
			pos(p->x, p->y);
			printf("  ");
			p->x += 2;
		}

		for (p = tray; p != NULL; p = p->next) {  //打印托盘
			pos(p->x,p->y);
			printf("■");
		}
	}
	if (keyup(VK_LEFT) && tray->x > 10) {
		for (p = tray; p != NULL; p = p->next) {  //清除原来的托盘，并把位置左移一格
			pos(p->x, p->y);
			printf("  ");
			p->x -= 2;
		}

		for (p = tray; p != NULL; p = p->next) {  //打印托盘
			pos(p->x, p->y);
			printf("■");
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
		nextball->x = ball->x + 2;
		nextball->y = ball->y - 2;
		nextball->angle = 1;
		for (p = first; p != NULL; p = p->next) {
			if (p->x == ball->x && p->y == ball->y - 1) {
				nextball->x = ball->x;
				nextball->y = ball->y;
				nextball->angle = 6;

				pos(ball->x, ball->y);
				printf("  ");
				free(ball);
				ball = nextball;
				pos(ball->x, ball->y);
				printf("●");
				return;

			}

		}

		for (p = first; p != NULL; p = p->next) {
			if (p->x == ball->x + 2 && p->y == ball->y - 1) {
				nextball->x = ball->x;
				nextball->y = ball->y - 1;
				nextball->angle = 12;

				pos(ball->x, ball->y);
				printf("  ");
				free(ball);
				ball = nextball;
				pos(ball->x, ball->y);
				printf("●");
				return;
			}

		}

		for (p = first; p != NULL; p = p->next) {

			if (p->x == ball->x + 2 && p->y == ball->y - 2) {
				nextball->x = ball->x + 2;
				nextball->y = ball->y - 1;
				nextball->angle = 6;

				pos(ball->x, ball->y);
				printf("  ");
				free(ball);
				ball = nextball;
				pos(ball->x, ball->y);
				printf("●");
				return;
			}


		}
		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;

	case 2:
		nextball->x = ball->x + 2;
		nextball->y = ball->y - 1;
		nextball->angle = 2;
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				block += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				block += 10;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				block += 100;
			}
		}
		switch (block) {
		case 111:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 8;
			break;
		case 101:
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 5;
			break;
		case 11:
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 11;
			break;
		case 1:
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
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 3:
		nextball->x = ball->x + 4;
		nextball->y = ball->y - 1;
		nextball->angle = 3;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	

	case 4:
		nextball->x = ball->x + 4;
		nextball->y = ball->y + 1;
		nextball->angle = 4;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 5:
		nextball->x = ball->x + 2;
		nextball->y = ball->y + 1;
		nextball->angle = 5;
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
				nextball->angle = rand() % 9 + 10;
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
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;



	case 6:
		nextball->x = ball->x + 2;
		nextball->y = ball->y + 2;
		nextball->angle = 6;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;

	case 7:
		nextball->x = ball->x - 2;
		nextball->y = ball->y + 2;
		nextball->angle = 7;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;



	case 8:
		nextball->x = ball->x - 2;
		nextball->y = ball->y + 1;
		nextball->angle = 8;
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
		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 9:
		nextball->x = ball->x - 4;
		nextball->y = ball->y + 1;
		nextball->angle = 9;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;

	case 10:
		nextball->x = ball->x - 4;
		nextball->y = ball->y - 1;
		nextball->angle = 10;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;


	case 11:
		nextball->x = ball->x - 2;
		nextball->y = ball->y - 1;
		nextball->angle = 11;
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
		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;




	case 12:
		nextball->x = ball->x - 2;
		nextball->y = ball->y - 2;
		nextball->angle = 12;
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("●");
		//printf("%d %d %d", ball->x, ball->y, ball->angle);
		return;



	}
}





void gameloop(void) {
	int n = 0;
	while (ball != NULL) {
		traymove();
		if (n % 20 == 0) {
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
	initbricks2();
	//inittray();
	initball();
	gameloop();
}