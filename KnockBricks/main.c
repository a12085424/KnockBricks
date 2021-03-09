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

typedef struct HittedBrick {
	Brick* q1;
	Brick* q10;
	Brick* q100;
	Brick* q1000;
	Brick* b1;
	Brick* b10;
	Brick* b100;
	Brick* b1000;
} HittedBrick;

/*全局变量*/
Brick* first_head;
Brick* first = NULL;  //第一块砖
Brick* wall = NULL;
Brick* node = NULL;
Brick* node2 = NULL;
Ball* balls[1000] = { NULL };
Brick* tray = NULL;
Brick* p = NULL;
Brick* q = NULL; 
Brick* r = NULL;  //托盘指针
HittedBrick* hitbrick;
int ballnumber;
int ballcount;
int endgamestatus;
int score;
int level;





/*声明所有函数*/
void pos(int x, int y);
void initmap(int level);
void initbricks(void);
void inittray(void);
void initball(void);
void traymove(void);
void ballmove(int ballnumber);
void brickbreak(Brick* b,Brick* q);
void endgame(int endgamestatus);
void gameloop(void);
void welcometogame(void);
void bonus(void);
void bonusball(int ballnumber);
void levelwelcome(void);

void pos(int x, int y) {
	COORD pos;
	HANDLE Houtput;
	pos.X = x;
	pos.Y = y;
	Houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(Houtput, pos);
}

void initmap(int level) {

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
	switch (level) {
	case 1:
		for (i = 10; i <= 46; i += 2) {
			nextwall = (Brick*)malloc(sizeof(Brick));
			nextwall->x = i;
			nextwall->y = 12;
			nextwall->next = wall;
			wall = nextwall;
		}
		break;
	case 2:
		for (i = 2; i <= 20; i += 2) {
			nextwall = (Brick*)malloc(sizeof(Brick));
			nextwall->x = i;
			nextwall->y = 12;
			nextwall->next = wall;
			wall = nextwall;
		}
		for (i = 36; i <= 54; i += 2) {
			nextwall = (Brick*)malloc(sizeof(Brick));
			nextwall->x = i;
			nextwall->y = 12;
			nextwall->next = wall;
			wall = nextwall;
		}
		break;
	case 3:
		for (i = 4; i <= 56; i += 4) {
			nextwall = (Brick*)malloc(sizeof(Brick));
			nextwall->x = i;
			nextwall->y = 12;
			nextwall->next = wall;
			wall = nextwall;
		}
		for (i = 2; i <= 54; i += 4) {
			nextwall = (Brick*)malloc(sizeof(Brick));
			nextwall->x = i;
			nextwall->y = 15;
			nextwall->next = wall;
			wall = nextwall;
		}
		break;
	}
	
	for (p = wall; p != NULL; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("□");
	}

}

void initbricks(void) {
	Brick* brick = NULL;
	int i;
	node = (Brick*)malloc(sizeof(Brick));
	first = (Brick*)malloc(sizeof(Brick));
	first->x = 2;
	first->y = 1;
	first->next = node;      //利用node连接砖和墙用于同时遍历
	node->x = 950414;
	node->y = 950414;
	node->next = wall;
	for (i = 4; i <= 54 * 6; i += 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = (i - 2) % 54 + 2;
		brick->y = (i - 2) / 54 + 1;
		brick->next = first;
		first = brick;
	}
	for (p = first; p->x != 950414; p = p->next) {
		pos(p->x, p->y);
		//printf("%d %d",p->x,p->y);
		printf("■");
	}
	

}

void initbricks2(void) {
	Brick* brick = NULL;
	int i;
	first = (Brick*)malloc(sizeof(Brick));
	node = (Brick*)malloc(sizeof(Brick));
	first->x = 40;
	first->y = 1;
	first->next = node;      //利用node连接砖和墙用于同时遍历
	node->x = 950414;
	node->y = 950414;
	node->next = wall;
	


	for (p = first; p->x != 950414; p = p->next) {
		pos(p->x, p->y);
		printf("■");
	}
}

void inittray(void) {
	Brick* nexttray;
	node2 = (Brick*)malloc(sizeof(Brick));
	tray = (Brick*)malloc(sizeof(Brick));
	int i;
	tray->x = 24;
	tray->y = 28;
	tray->next = node2;    //利用node2连接盘和砖和墙用于同时遍历
	node2->x = 950414;
	node2->y = 950414;
	node2->next = first;
	for (i = 26; i <= 32; i += 2) {
		nexttray = (Brick*)malloc(sizeof(Brick));
		nexttray->x = i;
		nexttray->y = 28;
		nexttray->next = tray;
		tray = nexttray;
	}
	for (p = tray; p->y != 950414; p = p->next) {
		pos(p->x, p->y);
		printf("■");
	}
}

void initball(void) {
	time_t t;
	int i;
	srand((unsigned)time(&t));
	for (i = 0; i <= ballcount - 1; i++) {
		printf("  ");
		balls[i] = NULL;
	}
	switch (level) {
	case 1:
		balls[0] = (Ball*)malloc(sizeof(Ball));
		balls[0]->x = 2 * (rand() % 10) + 20;
		balls[0]->y = 8;
		balls[0]->angle = (rand() % 6) + 4;
		break;
	case 2:
		balls[0] = (Ball*)malloc(sizeof(Ball));
		balls[0]->x = 28;
		balls[0]->y = 12;
		balls[0]->angle = (rand() % 3 + 1) + (rand() % 2) * 9;
		break;
	case 3:
		balls[0] = (Ball*)malloc(sizeof(Ball));
		balls[0]->x = 28;
		balls[0]->y = 13;
		balls[0]->angle = (rand() % 6) + 4;
		break;
	}
	
	pos(balls[0]->x, balls[0]->y);
	printf("●");
}

void bonusball(int ballnumber) {
	time_t t;
	srand((unsigned)time(&t) * ballnumber);
	balls[ballnumber] = (Ball*)malloc(sizeof(Ball));
	int i;
	for (i = ballcount - 1; i >= 0; i--) {
		if (balls[i] != NULL) {
			balls[ballnumber]->x = balls[i]->x;
			balls[ballnumber]->y = balls[i]->y;;
			balls[ballnumber]->angle = rand() % 12 + 1;
			pos(balls[ballnumber]->x, balls[ballnumber]->y);
			printf("●");
		}
	}
	
}

void traymove(void) {
	if (keyup(VK_RIGHT) && tray->x < 54) {
		for (r = tray; r->y != 950414; r = r->next) {  //清除原来的托盘，并把位置右移一格
			pos(r->x, r->y);
			printf("  ");
			r->x += 2;
		}

		for (r = tray; r->y != 950414; r = r->next) {  //打印托盘
			pos(r->x,r->y);
			printf("■");
		}
	}
	if (keyup(VK_LEFT) && tray->x > 10) {
		for (r = tray; r->y != 950414; r = r->next) {  //清除原来的托盘，并把位置左移一格
			pos(r->x, r->y);
			printf("  ");
			r->x -= 2;
		}

		for (r = tray; r->y != 950414; r = r->next) {  //打印托盘
			pos(r->x, r->y);
			printf("■");
		}
	}
}

void ballmove(int ballnumber) {
	Ball* nextball = NULL;
	nextball = (Ball*)malloc(sizeof(Ball));
	hitbrick = (HittedBrick*)malloc(sizeof(HittedBrick));
	first_head->next = first;
	hitbrick->b1 = NULL;
	hitbrick->b10 = NULL;
	hitbrick->b100 = NULL;
	hitbrick->b1000 = NULL;
	int wallblock = 0;
	int briblock = 0;
	time_t t;
	switch (balls[ballnumber]->angle) {
	case 1:
	
		nextball->x = balls[ballnumber]->x + 2;
		nextball->y = balls[ballnumber]->y - 2;
		nextball->angle = 1;

		//判断撞墙
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 2)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 6;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 12;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 6;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 11;
		}
		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		
		return;

	case 2:
		nextball->x = balls[ballnumber]->x + 2;
		nextball->y = balls[ballnumber]->y - 1;
		nextball->angle = 2;

		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
		case 111:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 8;
			break;
		case 11:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 11;
			break;
		case 101:
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 5;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = balls[ballnumber]->x + 2;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 3 + 4;
			}
			else if (rand() % 100 > 33) {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y - 1;
				nextball->angle = rand() % 3 + 10;
			}
			else {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 3 + 7;
			}
			break;
		case 10:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 10;
			}
			break;
		case 1:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 4;
			}
			break;
		default:
			break;

		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
		}
		switch (briblock) {
		case 111:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 11:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 110:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 101:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 100:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 10:
			brickbreak(hitbrick->b10, hitbrick->q10);
		case 1:
			brickbreak(hitbrick->b1, hitbrick->q1);
		default:
			break;
		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;


	case 3:
		nextball->x = balls[ballnumber]->x + 4;
		nextball->y = balls[ballnumber]->y - 1;
		nextball->angle = 3;
	
		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 4 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 10000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 11;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 4;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 11;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 5;
		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 4 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;


	

	case 4:
		nextball->x = balls[ballnumber]->x + 4;
		nextball->y = balls[ballnumber]->y + 1;
		nextball->angle = 4;
		
		//判断撞墙

		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 4 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 8;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 3;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 8;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 2;
		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 4 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}
		


		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;






	case 5:
		nextball->x = balls[ballnumber]->x + 2;
		nextball->y = balls[ballnumber]->y + 1;
		nextball->angle = 5;
		
		//判断撞墙

		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
		case 111:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 11;
			break;
		case 11:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 11;
			break;
		case 101:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 8;
			break;
		case 110:
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 2;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = balls[ballnumber]->x + 2;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 3 + 1;
			}
			else if (rand() % 100 > 33) {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y + 1;
				nextball->angle = rand() % 3 + 7;
			}
			else {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 3 + 10;
			}
			break;
		case 10:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 1;
			}
			break;
		case 1:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 7;
			}
			break;
		default:
			break;

		}
		
		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
		}
		switch (briblock) {
		case 111:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 11:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 101:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 110:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 100:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		default:
			break;

		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle,wallblock);
		return;



	case 6:
		nextball->x = balls[ballnumber]->x + 2;
		nextball->y = balls[ballnumber]->y + 2;
		nextball->angle = 6;

		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 2)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 1;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 7;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x + 2;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 1;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 8;
		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y + 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x + 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}

		
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;

	case 7:
		nextball->x = balls[ballnumber]->x - 2;
		nextball->y = balls[ballnumber]->y + 2;
		nextball->angle = 7;
		
		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 2)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 12;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 6;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 12;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 5;
		}
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}


		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;



	case 8:
		nextball->x = balls[ballnumber]->x - 2;
		nextball->y = balls[ballnumber]->y + 1;
		nextball->angle = 8;
	
		//判断撞墙

		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
		case 111:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 2;
			break;
		case 11:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 2;
			break;
		case 101:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 5;
			break;
		case 110:
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 11;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = balls[ballnumber]->x - 2;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 3 + 10;
			}
			else if (rand() % 100 > 33) {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y + 1;
				nextball->angle = rand() % 3 + 4;
			}
			else {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 9 + 1;
			}
			break;
		case 10:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 10;
			}
		case 1:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 4;
			}
			break;
		default:
			break;

		}
		
		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
		}
		switch (briblock) {
		case 111:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 11:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 101:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 110:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 100:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		default:
			break;

		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;


	case 9:
		nextball->x = balls[ballnumber]->x - 4;
		nextball->y = balls[ballnumber]->y + 1;
		nextball->angle = 9;
		
		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 4 && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 5;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 10;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y + 1;
			nextball->angle = 5;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 11;
		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 4 && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y + 1)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}
		

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;

	case 10:
		nextball->x = balls[ballnumber]->x - 4;
		nextball->y = balls[ballnumber]->y - 1;
		nextball->angle = 10;
		
		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 4 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 2;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 9;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 2;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 11;
		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 4 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q1000 = q;
				hitbrick->b1000 = p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;


	case 11:
		nextball->x = balls[ballnumber]->x - 2;
		nextball->y = balls[ballnumber]->y - 1;
		nextball->angle = 11;
	
		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
		case 111:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 5;
			break;
		case 11:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 5;
			break;
		case 101:
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 2;
			break;
		case 110:
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 8;
			break;
		case 100:
			srand((unsigned)time(&t));
			if (rand() % 100 > 66) {
				nextball->x = balls[ballnumber]->x - 2;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 3 + 7;
			}
			else if (rand() % 100 > 33) {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y - 1;
				nextball->angle = rand() % 3 + 1;
			}
			else {
				nextball->x = balls[ballnumber]->x;
				nextball->y = balls[ballnumber]->y;
				nextball->angle = rand() % 9 + 4;
			}
			break;
		case 10:
			srand((unsigned)time(&t));
			if (rand() % 2 == 1) {
				nextball->angle = rand() % 3 + 7;
			}
			break;
		case 1:
			srand((unsigned)time(&t));
			if (rand() % 2 >= 1) {
				nextball->angle = rand() % 3 + 1;
			}
			break;
		default:
			break;

		}

		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
		}
		switch (briblock) {
		case 111:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 11:
			brickbreak(hitbrick->b1, hitbrick->q1);
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 101:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 110:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 100:
			brickbreak(hitbrick->b100, hitbrick->q100);
			break;
		case 10:
			brickbreak(hitbrick->b10, hitbrick->q10);
			break;
		case 1:
			brickbreak(hitbrick->b1, hitbrick->q1);
		default:
			break;

		}

		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle);
		return;




	case 12:
		nextball->x = balls[ballnumber]->x - 2;
		nextball->y = balls[ballnumber]->y - 2;
		nextball->angle = 12;

		//判断撞墙
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 2)) {
				wallblock += 100;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y;
			nextball->angle = 7;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 1;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = balls[ballnumber]->x - 2;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 7;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = balls[ballnumber]->x;
			nextball->y = balls[ballnumber]->y - 1;
			nextball->angle = 2;
		}
		
		//判断撞砖
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == balls[ballnumber]->x && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y - 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == balls[ballnumber]->x - 2 && p->y == balls[ballnumber]->y)) {
				hitbrick->q1000 = q;
				hitbrick->b1000= p;
				briblock += 1000;
			}
		}
		if (briblock % 10 == 1) {
			brickbreak(hitbrick->b1, hitbrick->q1);
		}
		else if (briblock % 100 == 10) {
			brickbreak(hitbrick->b10, hitbrick->q10);
		}
		else if (briblock % 1000 == 100) {
			brickbreak(hitbrick->b100, hitbrick->q100);
		}
		else if (briblock % 10000 == 1000) {
			brickbreak(hitbrick->b1000, hitbrick->q1000);
		}


		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("  ");
		free(balls[ballnumber]);
		balls[ballnumber] = nextball;
		pos(balls[ballnumber]->x, balls[ballnumber]->y);
		printf("●");
		//printf("%d %d %d %d\n", balls[ballnumber]->x, balls[ballnumber]->y, balls[ballnumber]->angle,briblock);
		return;



	}
}

void bonus(void) {
	int i;
	bonusball(ballcount);
	ballcount++;
}

void brickbreak(Brick* b, Brick* q) {
	if (b == NULL) {
		return;
	}
	pos(b->x, b->y);
	printf("  ");
	if (b == first) {
		first = first->next;
		node2->next = first;
		score += 10;
		
	}
	else {
		q->next = b->next;
		score += 10;
	}
	time_t t;
	srand((unsigned)time(&t));
	if (rand() % 3 == 1) {
		bonus();
	}
	
}



void endgame(int endgamestatus) {
	system("cls");
	pos(32,15);
	switch (endgamestatus) {
	case 1:
		printf("您死了，您的得分是：%d", score);
		break;
	case 2:
		printf("过关！您的得分是：%d", score);
		break;
	case 3:
		printf("您已通关！您的得分是：%d", score);
		break;
	}
	while (1) {
		if (keyup(VK_RETURN)) {
			break;
		}
		pos(36, 25);
		printf("<按回车键继续>");
		pos(99, 34);
		Sleep(1000);
		pos(36, 25);
		printf("                           ");
		pos(99, 34);
		Sleep(500);
	}
	system("cls");
	
}

void welcometogame(void) {
	pos(44, 10);
	printf("打砖块");
	while (1) {
		if (keyup(VK_RETURN)) {
			break;
		}
		pos(38, 20);
		printf("<按回车键开始游戏>");
		pos(99, 34);
		Sleep(1000);
		pos(38, 20);
		printf("                      ");
		pos(99, 34);
		Sleep(500);
	}
	system("cls");
}

void levelwelcome(void) {
	int i;
	for (i = 1; i <= 3; i++) {
		if (keyup(VK_RETURN)) {
			break;
		}
		pos(26, 20);
		printf("第%d关", level);
		pos(99, 34);
		Sleep(1000);
		pos(26, 20);
		printf("                 ");
		pos(99, 34);
		Sleep(500);
	}

}


void gameloop(void) {
	int n = 0;
	endgamestatus = 0;
	score = 0;
	ballcount = 1;
	int i;

	first_head = (Brick*)malloc(sizeof(Brick));
	pos(65, 10);
	printf("玩法说明：");
	pos(65, 12);
	printf("按←→控制方向接住小球");
	pos(65, 14);
	printf("击碎砖块有概率增加小球！");
	pos(65, 16);
	printf("击碎所有砖块获得胜利！");
	pos(70, 30);
	printf("by黄方");
	while ( 1 ) {
		
		traymove();
		if (n % 70 == 0) {
			int i;
			for (i = 0; i <= ballcount - 1; i++) {
				if (balls[i] != NULL) {
					if (balls[i]->y > 30 || balls[i]->x > 58) {
						pos(balls[i]->x, balls[i]->y);
						printf("  ");
						balls[i] = NULL;
						continue;
					}
					ballmove(i);
				}
				
			}
		}
		pos(99, 34);
		pos(65, 5);
		printf("当前得分：%d", score);
		pos(99, 34);
		Sleep(1);
		n++;
		for (i = 0; i <= ballcount - 1; i++) {
			endgamestatus = 1;
			if (balls[i] != NULL) {
				endgamestatus = 0;
				break;
			}
		}
		if (first->y == 950414) {
			endgamestatus = 2;
			if (level == 3) {
				endgamestatus = 3;
			}
		}
		if (endgamestatus) {
			endgame(endgamestatus);
			break;
		}
		
	}
	
}


int main(void) {
	system("mode con cols=100 lines=35");

	HWND hWnd = GetConsoleWindow(); //获得cmd窗口句柄
	RECT rc;
	GetWindowRect(hWnd, &rc); //获得cmd窗口对应矩形

	//改变cmd窗口风格
	SetWindowLongPtr(hWnd,
		GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	//因为风格涉及到边框改变，必须调用SetWindowPos，否则无效果
	SetWindowPos(hWnd,
		NULL,
		rc.left,
		rc.top,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL);

	welcometogame();
	for (level = 3; level <= 3;) {
		initball(level);
		initmap(level);
		initbricks();
		inittray();
		levelwelcome();
		pos(60, 29);
		gameloop();
		if (endgamestatus == 2) {
			level++;
		}
		else {
			level = 1;
		}
		endgamestatus = 0;
	}
	
}