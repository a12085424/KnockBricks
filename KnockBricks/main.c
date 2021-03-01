#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define keyup(VK_CODE) ((GetAsyncKeyState(VK_CODE) & 0x0001) ?1:0) 

typedef struct Brick {   //砖块结构声明
	int x;
	int y;
	struct Brick *next;
} Brick;

/*全局变量*/
Brick* first = NULL;  //第一块砖
Brick* ball = NULL;
Brick* tray = NULL;
Brick* p = NULL;



/*声明所有函数*/
void pos(int x, int y);
void initmap(void);
void initbricks(void);
void inittray(void);
void initball(void);
void traymove(void);
void ballmove(void);
void ballbounce(void);
void brickbreak(void);
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
	ball->x = 28;
	ball->y = 15;
	ball->next = NULL;
	pos(ball->x, ball->y);
	printf("●");
}

void traymove(void) {
	if (keyup(VK_RIGHT)) {
		for (p = tray; p != NULL; p = p->next) {
			pos(p->x, p->y);
			printf("  ");
			p->x += 2;
		}
		//pos(0, 28);
		//printf("                                                      ");
		for (p = tray; p != NULL; p = p->next) {
			pos(p->x,p->y);
			printf("■");
		}
	}
	if (keyup(VK_LEFT)) {
		for (p = tray; p != NULL; p = p->next) {
			pos(p->x, p->y);
			printf("  ");
			p->x -= 2;
		}
		//pos(0, 28);
		//printf("                                                      ");
		for (p = tray; p != NULL; p = p->next) {
			pos(p->x, p->y);
			printf("■");
		}
	}
}







void gameloop(void) {
	while (1) {
		traymove();
		pos(60, 29);
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