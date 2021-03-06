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

/*È«¾Ö±äÁ¿*/
Brick* first_head;
Brick* first = NULL;  //µÚÒ»¿é×©
Brick* wall = NULL;
Brick* node = NULL;
Brick* node2 = NULL;
Ball* ball = NULL;
Brick* tray = NULL;
Brick* p = NULL;
Brick* q = NULL; 
Brick* r = NULL;  //ÍÐÅÌÖ¸Õë
HittedBrick* hitbrick;




/*ÉùÃ÷ËùÓÐº¯Êý*/
void pos(int x, int y);
void initmap(void);
void initbricks(void);
void inittray(void);
void initball(void);
void traymove(void);
void ballmove(void);
Ball* ballbounce(Ball* next);
void brickbreak(Brick* b,Brick* q);
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
	node = (Brick*)malloc(sizeof(Brick));
	first = (Brick*)malloc(sizeof(Brick));
	first->x = 2;
	first->y = 1;
	first->next = node;      //ÀûÓÃnodeÁ¬½Ó×©ºÍÇ½ÓÃÓÚÍ¬Ê±±éÀú
	node->x = 950414;
	node->y = 950414;
	node->next = wall;
	for (i = 4; i <= 54 * 4; i += 2) {
		brick = (Brick*)malloc(sizeof(Brick));
		brick->x = (i - 2) % 54 + 2;
		brick->y = (i - 2) / 54 + 1;
		brick->next = first;
		first = brick;
	}
	for (p = first; p->x != 950414; p = p->next) {
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
	node2 = (Brick*)malloc(sizeof(Brick));
	tray = (Brick*)malloc(sizeof(Brick));
	int i;
	tray->x = 24;
	tray->y = 28;
	tray->next = node2;    //ÀûÓÃnode2Á¬½ÓÅÌºÍ×©ºÍÇ½ÓÃÓÚÍ¬Ê±±éÀú
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
		printf("¡ö");
	}
}

void initball(void) {
	ball = (Brick*)malloc(sizeof(Brick));
	ball->x = 4;
	ball->y = 10;
	ball->angle = 3;
	pos(ball->x, ball->y);
	printf("¡ñ");
}

void traymove(void) {
	if (keyup(VK_RIGHT) && tray->x < 54) {
		for (r = tray; r->y != 950414; r = r->next) {  //Çå³ýÔ­À´µÄÍÐÅÌ£¬²¢°ÑÎ»ÖÃÓÒÒÆÒ»¸ñ
			pos(r->x, r->y);
			printf("  ");
			r->x += 2;
		}

		for (r = tray; r->y != 950414; r = r->next) {  //´òÓ¡ÍÐÅÌ
			pos(r->x,r->y);
			printf("¡ö");
		}
	}
	if (keyup(VK_LEFT) && tray->x > 10) {
		for (r = tray; r->y != 950414; r = r->next) {  //Çå³ýÔ­À´µÄÍÐÅÌ£¬²¢°ÑÎ»ÖÃ×óÒÆÒ»¸ñ
			pos(r->x, r->y);
			printf("  ");
			r->x -= 2;
		}

		for (r = tray; r->y != 950414; r = r->next) {  //´òÓ¡ÍÐÅÌ
			pos(r->x, r->y);
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
	hitbrick = (HittedBrick*)malloc(sizeof(HittedBrick));
	first_head->next = first;
	hitbrick->b1 = NULL;
	hitbrick->b10 = NULL;
	hitbrick->b100 = NULL;
	hitbrick->b1000 = NULL;
	int wallblock = 0;
	int briblock = 0;
	time_t t;
	switch (ball->angle) {
	case 1:
	
		nextball->x = ball->x + 2;
		nextball->y = ball->y - 2;
		nextball->angle = 1;

		//ÅÐ¶Ï×²Ç½
		for (p = first; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				wallblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 2)) {
				wallblock += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 6;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 12;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y - 1;
			nextball->angle = 6;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 11;
		}
		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
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

		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				wallblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				wallblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
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

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
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
	
		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y - 1)) {
				wallblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				wallblock += 10000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 10;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y - 1;
			nextball->angle = 10;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 5;
		}

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y - 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
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
		
		//ÅÐ¶Ï×²Ç½

		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y + 1)) {
				wallblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 3;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 9;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y;
			nextball->angle = 2;
		}

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x + 4 && p->y == ball->y + 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
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
		
		//ÅÐ¶Ï×²Ç½

		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
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
		
		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
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

		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d %d", ball->x, ball->y, ball->angle,wallblock);
		return;



	case 6:
		nextball->x = ball->x + 2;
		nextball->y = ball->y + 2;
		nextball->angle = 6;

		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				wallblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 2)) {
				wallblock += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 1;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 7;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x + 2;
			nextball->y = ball->y + 1;
			nextball->angle = 1;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 8;
		}

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y + 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x + 2 && p->y == ball->y)) {
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
		
		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				wallblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 2)) {
				wallblock += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 12;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 6;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y + 1;
			nextball->angle = 12;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y + 1;
			nextball->angle = 5;
		}
		pos(ball->x, ball->y);
		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
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
	
		//ÅÐ¶Ï×²Ç½

		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
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
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
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
		
		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y + 1)) {
				wallblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 4;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 10;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y + 1;
			nextball->angle = 4;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
		}

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y + 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y + 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y + 1)) {
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
		
		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y - 1)) {
				wallblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 3;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 9;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y - 1;
			nextball->angle = 3;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y;
			nextball->angle = 11;
		}

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x - 4 && p->y == ball->y - 1)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
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
	
		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				wallblock += 1;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				wallblock += 100;
			}
		}
		switch (wallblock) {
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

		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
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

		//ÅÐ¶Ï×²Ç½
		for (p = tray; p != NULL; p = p->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				wallblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				wallblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 2)) {
				wallblock += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
				wallblock += 1000;
			}
		}
		if (wallblock % 10 == 1) {
			nextball->x = ball->x;
			nextball->y = ball->y;
			nextball->angle = 7;
		}
		else if (wallblock % 100 == 10) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 1;
		}
		else if (wallblock % 1000 == 100) {
			nextball->x = ball->x - 2;
			nextball->y = ball->y - 1;
			nextball->angle = 7;
		}
		else if (wallblock % 10000 == 1000) {
			nextball->x = ball->x;
			nextball->y = ball->y - 1;
			nextball->angle = 2;
		}
		
		//ÅÐ¶Ï×²×©
		for (p = first, q = first_head; p->y != 950414; p = p->next, q = q->next) {
			if ((p->x == ball->x && p->y == ball->y - 1)) {
				hitbrick->q1 = q;
				hitbrick->b1 = p;
				briblock += 1;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 1)) {
				hitbrick->q10 = q;
				hitbrick->b10 = p;
				briblock += 10;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y - 2)) {
				hitbrick->q100 = q;
				hitbrick->b100 = p;
				briblock += 100;
			}
			if ((p->x == ball->x - 2 && p->y == ball->y)) {
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


		pos(ball->x, ball->y);
		printf("  ");
		free(ball);
		ball = nextball;
		pos(ball->x, ball->y);
		printf("¡ñ");
		//printf("%d %d %d %d\n", ball->x, ball->y, ball->angle,briblock);
		return;



	}
}


void brickbreak(Brick* b, Brick* q) {
	pos(b->x, b->y);
	printf("  ");
	q->next = b->next;
	//free(b);
}






void gameloop(void) {
	int n = 0;
	first_head = (Brick*)malloc(sizeof(Brick));
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
	initmap();
	initbricks();
	inittray();
	initball();
	pos(60, 29);
	
	gameloop();
}