#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <thread>
#include <mutex>
#include "ScoreBoard.h"

#define screen_x 120
#define screen_y 40

HANDLE wHnd;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x-1,screen_y-1 };






void getItemSound()
{
	Beep(900, 70);
	Beep(1000, 70);
}

void HitSound()
{
	Beep(400, 70);
	Beep(300, 70);
}

void BsHit()
{
	Beep(600,70);
	Beep(700, 70);
}

struct Bullet
{
	int status = 0;
	int x = 0, y = 0;
}bs[50];

struct Enemy
{
	int x = 0, y = 0;
	int status = 0;
	int hp = 10;
	int dmg = 10;
}enemy[10];

struct Enemy2
{
	int hp = 30;
	int x = 0, y = 0;
	int status = 0;
	int dmg = 10;
}enemy2[10];

struct Missile
{
	int hp = 10;
	int x = 0, y = 0;
	int dx = 0;
	int status = 0;
	int dmg = 10;
}missile[5];

struct Beam
{
	int x = 0, y = 0;
	int status = 0;
	int dmg = 10;
}beam[2];



struct Ship
{
	int x = 10, y = 20;
	int hp = 50;
	int status = 0;
}ship;

struct Heal
{
	int x = 5;
	int y = 3;
	int status = 0;

}heal;

struct Gold
{
	int x = 5;
	int y = 3;
	int status = 0;
	unsigned long time = 0;

}gold;

unsigned long frame = 1;
int i;
int current = 0;
int maxenemy = 2;
int maxenemy2 = 0;
int maxmissile = 3;
int changelevel = 100;
int level = 1;
int page = 0;
int pointer = 1;
int pointx = 49, pointy = 20;
char name[20];
int mycolor = 2;

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void border()
{
	setcolor(0, 7);
	for (int i = 0; i < 120; i++)
	{
		gotoxy(i, 3); printf(" ");
		gotoxy(i, 39); printf(" ");
	}
	for (int j = 4; j < 39; j++)
	{
		gotoxy(0, j); printf(" ");
		gotoxy(119, j); printf(" ");
	}
}

void clear_screen()
{
	setcolor(7, 0);
	for ( int i = 1; i < 119; i++)
	{
		for (int j = 4; j < 39 ; j++)
		{
			gotoxy(i, j); printf(" ");
		}
	}
}

void print_menu()
{
	setcolor(7, 0);
	if (frame != 1)
	{
		gotoxy(57, 18); printf("RESUME");
	}
	gotoxy(53, 20); printf("START NEW GAME");
	gotoxy(55, 22); printf("SCORE BOARD");
	gotoxy(48, 30); printf("PRESS SPACEBAR TO SELECT");
}

void clear_menu()
{
	setcolor(7, 0);
	gotoxy(50, 20); printf("          ");
	gotoxy(50, 22); printf("           ");
}

void print_pointer(int x,int y)
{
	setcolor(4, 0);
	gotoxy(x, y); printf(">>>");
}

void clear_pointer(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y);
	printf("   ");
}

void draw_ship(int x, int y)
{
	if (ship.status==0){setcolor(mycolor, 0);}
	if (ship.status == 1) { setcolor(6, 0); }
	
	gotoxy(x, y - 1); printf("   _||==0  ");
	gotoxy(x, y);   printf(" _| ==\\___");
	gotoxy(x, y + 1); printf("(__0__0___)");

}

void erase_ship(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y - 1); printf("             ");
	gotoxy(x, y); printf("              ");
	gotoxy(x, y + 1); printf("             ");
}

void flashing(int x, int y)
{
	if (ship.status == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			Sleep(100);
			mycolor = 0;
			Sleep(100);
			mycolor = 2;
		}
	}
	
}

void draw_bullet(int x, int y)
{
	setcolor(2, 0);
	gotoxy(x, y); printf(">==>");
}

void clear_bullet(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y); printf("    ");
}

void draw_enemy(int x, int y)
{
	setcolor(4, 0);
	gotoxy(x + 3, y - 1); printf("____");
	gotoxy(x, y);   printf("__/*===|__");
	gotoxy(x, y + 1); printf("\\________/");
}

void draw_enemy2(int x, int y)
{
	setcolor(5, 0);
	gotoxy(x + 3, y - 1); printf("____");
	gotoxy(x, y);   printf("__/*===|__");
	gotoxy(x, y + 1); printf("\\________/");
}

void clear_enemy_hit_bullet(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y - 2); printf("              ");
	gotoxy(x, y - 1); printf("              ");
	gotoxy(x, y);	  printf("              ");
	gotoxy(x, y + 1); printf("              ");
	gotoxy(x, y + 2); printf("              ");
}

void clear_enemy(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y - 1); printf("          ");
	gotoxy(x, y);	  printf("          ");
	gotoxy(x, y + 1); printf("          ");
}

void draw_missile(int x, int y)
{
	setcolor(4, 0);
	gotoxy(x, y); printf("(000{");
}

void clear_missile(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y); printf("     ");
}

void draw_beam(int x, int y)
{
	setcolor(9, 0);
	gotoxy(x, y); printf("C888<");
}

void clear_beam(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y); printf("     ");
}

void draw_heal(int x,int y)
{
	setcolor(2, 0);
	gotoxy(x, y); printf("(H)");
}

void draw_gold(int x, int y)
{
	setcolor(6, 0);
	gotoxy(x, y); printf("(G)");
}

void clear_item(int x, int y)
{
	setcolor(7, 0);
	gotoxy(x, y); printf("   ");
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}

char attribute(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	int buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputAttribute(hStd, (LPWORD)buf, 1, c, (LPDWORD)&num_read))
	{

		return '\0';
	}
	else
		return buf[0];

}



int score = 0;
void print_score(int x)
{
	setcolor(2, 0);
	gotoxy(109, 0); printf("Score : %d", x);
	if (x >= 1000)
	{
		gotoxy(109, 0); printf("          ");
		gotoxy(108, 0); printf("Score : %d", x);
	}
	
}


void print_hp(int x)
{
	setcolor(4, 0);
	gotoxy(4, 0); printf("    ", x);
	gotoxy(0, 0); printf("HP : %d ", x);
}

void print_level(int x)
{
	setcolor(4, 0);
	gotoxy(10, 0); printf("Level : %d", x);
}

void print_frame(int x)
{
	setcolor(4, 0);
	gotoxy(50, 0); printf("frame : %d", x);
}

void gameover() {
	setcolor(4, 0);
	printf("GAME OVER");
}

int xy[2];int counter = 0;int around = 0;
int* checkotherenemy(int x, int y)
{
	while (around == 0)
	{
		int endx = x + 10;
		int endy = y + 7;
		for (int i = x - 10; i <= endx; i++)
		{
			for (int j = y - 7; j <= endy; j++)
			{
				char checkenemy = cursor(x,y);
				//char checkenemy = attribute(x, y);
				if (checkenemy == ' ')
				{
				
				}
			}
		}
		if (counter == 0)
		{
			xy[0] = x;
			xy[1] = y;
			return xy;
			around = 1;
		}
		else
		{
			x = 90 + (rand() % 17);
			y = 5 + (rand() % 32);
			counter = 0;
		}
		
	}
	
	
}

int newex, newey;
int* rnewxy;
void rerandomenamyafterhit(int c) {
	enemy[c].status = 0;
	newex = 106;
	newey = 5 + (rand() % 32);
	rnewxy = checkotherenemy(newex, newey);
	enemy[c].x = *(rnewxy + 0);
	enemy[c].y = *(rnewxy + 1);
	draw_enemy(enemy[c].x, enemy[c].y);
	enemy[c].status = 1;
	enemy[c].hp = 10;
	if (level >= 7)
	{
		enemy[c].hp = 30;
		enemy[c].dmg = 20;
	}
}

void rerandomenamy2afterhit(int c) {
	enemy2[c].status = 0;
	newex = 106;
	newey = 5 + (rand() % 32);
	rnewxy = checkotherenemy(newex, newey);
	enemy2[c].x = *(rnewxy + 0);
	enemy2[c].y = *(rnewxy + 1);
	draw_enemy2(enemy2[c].x, enemy2[c].y);
	enemy2[c].status = 1;
	enemy2[c].hp = 30;
	if (level >= 7)
	{
		enemy2[c].hp = 50;
		enemy2[c].dmg = 30;
	}
}

int newmx, newmy;
int* rnewmxy;
void rerandommissileafterhit(int c) {
	missile[c].status = 0;
	newmx = 110;
	newmy = 5 + (rand() % 32);
	rnewmxy = checkotherenemy(newmx, newmy);
	missile[c].x = *(rnewmxy + 0);
	missile[c].y = *(rnewmxy + 1);
	draw_missile(missile[c].x, missile[c].y);
	missile[c].status = 1;
}

void reset()
{
	clear_screen();
	frame = 1;
	level = 1;
	ship.hp = 50;
	score = 0;
	maxenemy = 2;
	maxenemy2 = 0;
	maxmissile = 3;
	heal.status = 0;
	gold.status = 0;
	for (i = 0; i < maxenemy; i++)
	{
		enemy[i].status = 0;
	}
	for (i = 0; i < maxenemy2; i++)
	{
		enemy[i].status = 0;
	}
	for (i = 0; i < maxmissile; i++)
	{
		missile[i].status = 0;
	}
	for (i = 0; i < 2; i++)
	{
		beam[i].status = 0;
	}
	for ( i = 0; i < current; i++)
	{
		bs[i].status = 0;
	}
}



int main()
{
	char ch = ' ';
	setcursor(0);
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	border();
	do {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a')
			{
				if (page == 1)
				{
					if (ship.x > 0)
					{
						erase_ship(ship.x, ship.y); draw_ship(--ship.x, ship.y);
					}
					else { draw_ship(ship.x, ship.y); }
				}
			}
			if (ch == 'd')
			{
				if (page == 1)
				{
					if (ship.x < 108)
					{
						erase_ship(ship.x, ship.y);
						draw_ship(++ship.x, ship.y);
					}
					else { draw_ship(ship.x, ship.y); }
				}
			}
			if (ch == 's')
			{
				if (page == 0)
				{
					clear_pointer(pointx, pointy);
					if (pointy != 22) { pointy = pointy + 2; }
					if (pointy == 20) { pointx = 49; }
					if (pointy == 22) { pointx = 51; }
					print_pointer(pointx, pointy);
				}
				if (page == 1)
				{
					if (ship.y < 37)
					{
						erase_ship(ship.x, ship.y);
						draw_ship(ship.x, ++ship.y);
					}
					else { draw_ship(ship.x, ship.y); }
				}
			}
			if (ch == 'w')
			{
				if (page == 0)
				{
					clear_pointer(pointx, pointy);
					if (frame == 1)
					{
						if (pointy != 20) { pointy = pointy - 2; }
					}
					else { if (pointy != 18) { pointy = pointy - 2; }  }
					if (pointy == 18) { pointx = 53; }
					if (pointy == 20) { pointx = 49; }
					print_pointer(pointx, pointy);
					
				}
				if (page == 1)
				{
					if (ship.y > 5)
					{
						erase_ship(ship.x, ship.y);
						draw_ship(ship.x, --ship.y);
					}
					else { draw_ship(ship.x, ship.y); }
				}
			}
			if (ch == ' ' )
			{
				if (page == 1 && bs[current].status == 0 && ship.status==0)
				{
					bs[current].status = 1;
					bs[current].x = ship.x + 12;
					bs[current].y = ship.y + 1;
					current = (current + 1) % 50;
				}
				if (page == 0)
				{
					if (pointy == 18)//point_resume
					{
						clear_screen();
						page = 1;
					}
					if (pointy == 20)//point_start
					{
						clear_screen();
						reset();
						page = 1;
					}
					if (pointy == 22)//point_scoreboard
					{
						clear_screen();
						page = 2;
					}
				}
			}
			if (ch =='e')
			{
				if (page == 2)
				{
					clear_screen();
					page = 0;
				}
				if (page == 3)
				{
					clear_screen();
					page = 0;
				}
			}
			if (ch == 'p')
			{
				if (page == 1)
				{
					clear_screen();
					page = 0;
				}
			}
			fflush(stdin);
		}
		if (page == 0)//menu 
		{
			
			print_pointer(pointx,pointy);
			print_menu();
		}
		if (page == 1)//gamestart
		{
			//clear_menu();
			//clear_pointer(pointx,pointy);
			
			draw_ship(ship.x, ship.y);
			print_score(score);
			print_hp(ship.hp);
			print_level(level);
			print_frame(frame);
			//สุ่มเกิดศัตรู
			for (i = 0; i < maxenemy; i++)
			{
				if (enemy[i].status == 0)
				{
					enemy[i].x = 106;
					enemy[i].y = 5 + (rand() % 32);
					checkotherenemy(enemy[i].x, enemy[i].y);
					draw_enemy(enemy[i].x, enemy[i].y);
					enemy[i].status = 1;
					//enemy[i].hp = 10;
					if (level>=7)
					{
						enemy[i].hp = 30;
						enemy[i].dmg = 20;
					}
				}
			}
			//สุ่มเกิด enemy2
			for (i = 0; i < maxenemy2; i++)
			{
				if (enemy2[i].status == 0)
				{
					enemy2[i].x = 106;
					enemy2[i].y = 5 + (rand() % 32);
					checkotherenemy(enemy2[i].x, enemy2[i].y);
					draw_enemy2(enemy2[i].x, enemy2[i].y);
					enemy2[i].status = 1;
					if (level >= 7)
					{
						enemy2[i].hp = 50;
						enemy2[i].dmg = 30;
					}
				}
			}
			//สุ่มเกิดmissile
			for (i = 0; i < maxmissile; i++)
			{
				if (missile[i].status == 0)
				{
					if (frame % 300 == 0)
					{
						missile[i].x = 114;
						missile[i].y = 5 + (rand() % 32);
						draw_missile(missile[i].x, missile[i].y);
						missile[i].status = 1;
						if (level >= 7)
						{
							missile[i].dmg = 20;
						}
					}
				}
			}
			//สุ่มเกิดbeam
			for ( i = 0; i < 2; i++)
			{
				if (beam[i].status == 0)
				{
					if (frame%500==0)
					{
						beam[i].x = 114;
						beam[i].y = 5 + (rand() % 32);
						draw_beam(beam[i].x, beam[i].y);
						beam[i].status = 1;
						if (level >= 7)
						{
							beam[i].dmg =30;
						}
					}
				}
			}

			//bullet 

			for (int i = 0; i < 50; i++)
			{
				if (bs[i].status == 1)
				{
					clear_bullet(bs[i].x, bs[i].y);
					if (bs[i].x > 114)
					{
						bs[i].status = 0;
					}
					else
					{
						draw_bullet(++bs[i].x, bs[i].y);
						for (int e = 0; e < maxenemy; e++)//enemy1
						{
							if (abs(enemy[e].y - bs[i].y) <= 1 && abs((enemy[e].x + 5) - (bs[i].x + 4)) < 5)
							{
								clear_bullet(bs[i].x, bs[i].y);

								enemy[e].hp -= 10;
								bs[i].status = 0;

							}
							if (enemy[e].hp == 0)
							{
								clear_enemy(enemy[e].x, enemy[e].y);
								std::thread q(BsHit);
								q.detach();
								score += 10;
								//enemy[e].status = 0;
								rerandomenamyafterhit(e);
							}
						}
						for (int e = 0; e < maxenemy2; e++)//enemy2
						{
							if (abs(enemy2[e].y - bs[i].y) <= 1 && abs((enemy2[e].x + 5) - (bs[i].x + 4)) < 5)
							{
								clear_bullet(bs[i].x, bs[i].y);
								std::thread q(BsHit);
								q.detach();
								enemy2[e].hp -= 10;
								bs[i].status = 0;

							}
							if (enemy2[e].hp == 0)
							{
								clear_enemy(enemy2[e].x, enemy2[e].y);
								score += 10;
								//enemy2[e].status = 0;
								rerandomenamy2afterhit(e);
							}
						}
					}
				}
			}

			//enemy_move

			for (int e = 0; e < maxenemy; e++)
			{
				if (enemy[e].status == 1) {
					if (enemy[e].x <= 1)//ชนขอบ
					{
						clear_enemy(enemy[e].x, enemy[e].y);
						//rerandomenamyafterhit(e);
						enemy[e].status = 0;
					}
					else
					{
						if (abs(enemy[e].y - ship.y) <= 2 && abs((enemy[e].x) - (ship.x + 11)) < 2)
						{
							clear_enemy(enemy[e].x, enemy[e].y);
							enemy[e].status = 0;
							if (ship.status == 0)
							{
								ship.hp = ship.hp - enemy[e].dmg;
								std::thread p(flashing,ship.x,ship.y);
								p.detach();
								std::thread q(HitSound);
								q.detach();
								
							}
							if (ship.status==1)
							{
								std::thread q(BsHit);
								q.detach();
								score += 10;
								ship.hp += 5;
							}
							//rerandomenamyafterhit(e);
							
						}
						else
						{
							if (frame % 3 == 0)
							{
								clear_enemy(enemy[e].x, enemy[e].y); draw_enemy(--enemy[e].x, enemy[e].y);
							}
						}
					}
				}
			}
			//enemy2_move
			for (int e = 0; e < maxenemy2; e++)
			{
				if (enemy2[e].status == 1) {
					if (enemy2[e].x <= 0)//ชนขอบ
					{
						clear_enemy(enemy2[e].x, enemy2[e].y);
						enemy2[e].status = 1;
						//rerandomenamy2afterhit(e);
					}
					else
					{
						if (abs(enemy2[e].y - ship.y) <= 2 && abs((enemy2[e].x) - (ship.x + 11)) < 2)//ชนยาน
						{
							clear_enemy(enemy2[e].x, enemy2[e].y);
							enemy2[e].status = 0;
							if (ship.status == 0)
							{
								ship.hp = ship.hp - enemy2[e].dmg;
								std::thread p(flashing, ship.x, ship.y);
								p.detach();
								std::thread q(HitSound);
								q.detach();
							}
							if (ship.status == 1)//gold
							{
								std::thread q(BsHit);
								q.detach();
								score += 10;
								ship.hp += 5;
							}
							//rerandomenamy2afterhit(e);
						}
						else
						{
							if (frame % 2 == 0)
							{
								clear_enemy(enemy2[e].x, enemy2[e].y); draw_enemy2(--enemy2[e].x, enemy2[e].y);
							}
						}
					}
				}
			}
			//missile_move
			for (int e = 0; e < maxmissile; e++)
			{
				if (missile[e].status == 1) {
					if (missile[e].x <= 1)//ชนขอบ
					{
						clear_missile(missile[e].x, missile[e].y);
						missile[e].status = 0;
						//rerandommissileafterhit(e);
					}
					else
					{
						if (abs(missile[e].y - ship.y) <= 1 && abs((missile[e].x) - (ship.x + 11)) < 2)
						{
							clear_missile(missile[e].x, missile[e].y);
							missile[e].status = 0;
							if (ship.status == 0)
							{
								ship.hp = ship.hp - 10;
								std::thread p(flashing, ship.x, ship.y);
								p.detach();
								std::thread q(HitSound);
								q.detach();
							}
							if (ship.status == 1)//gold
							{
								std::thread q(BsHit);
								q.detach();
								score += 10;
								ship.hp += 5;
							}
							//rerandommissileafterhit(e);
						}
						else
						{
							if (missile[e].y < ship.y)
							{
								if (frame % 7 == 0)
								{
									clear_missile(missile[e].x, missile[e].y);
									if (missile[e].dx < 5)
									{
										draw_missile(--missile[e].x, missile[e].y); missile[e].dx += 1;
									}
									else { draw_missile(--missile[e].x, ++missile[e].y); missile[e].dx = 0; }
								}
							}
							else if (missile[e].y > ship.y)
							{
								if (frame % 7 == 0)
								{
									clear_missile(missile[e].x, missile[e].y);
									if (missile[e].dx < 3)
									{
										draw_missile(--missile[e].x, missile[e].y); missile[e].dx += 1;
									}
									else { draw_missile(--missile[e].x, --missile[e].y); missile[e].dx = 0; }
								}
							}
							else if (frame % 10 == 0)
							{
								clear_missile(missile[e].x, missile[e].y);
								draw_missile(--missile[e].x, missile[e].y);
							}
						}
					}
				}
			}
			//beam_move
			for (int e = 0; e < 2; e++)
			{
				if (beam[e].status == 1) {
					if (beam[e].x <= 1)//ชนขอบ
					{
						clear_beam(beam[e].x, beam[e].y);
						beam[e].status = 0;
					}
					else
					{
						if (abs(beam[e].y - ship.y) <= 2 && abs((beam[e].x) - (ship.x + 11)) < 2)
						{
							clear_beam(beam[e].x, beam[e].y);
							beam[e].status = 0;
							if (ship.status == 0)
							{
								ship.hp = ship.hp - 10;
								std::thread p(flashing, ship.x, ship.y);
								p.detach();
								std::thread q(HitSound);
								q.detach();
							}
							if (ship.status == 1)//gold
							{
								std::thread q(BsHit);
								q.detach();
								score += 10;
								ship.hp += 5;
							}
						}
						else
						{
							clear_beam(beam[e].x, beam[e].y); draw_beam(--beam[e].x, beam[e].y);
						}
					}
				}
			}
			
			//item drop 
			if (frame % 700 == 0 && heal.status == 0)
			{
				heal.x = 20 + (rand() % 70);
				heal.y = 4;
				draw_heal(heal.x, heal.y);
				heal.status = 1;
			}

			if (frame % 2000 == 0 && gold.status == 0)
			{
				gold.x = 20 + (rand() % 70);
				gold.y = 4;
				draw_gold(gold.x, gold.y);
				gold.status = 1;
			}
			//heal
			if (heal.status == 1)
			{
				if (heal.y >= 38)
				{
					clear_item(heal.x, heal.y);
					heal.status = 0;
				}
				else
				{
					if (frame % 30 == 0)
					{
						clear_item(heal.x, heal.y);
						draw_heal(heal.x, ++heal.y);
					}
					if (abs(heal.y - ship.y) < 4 && abs((ship.x + 6) - heal.x) < 7)
					{
						clear_item(heal.x, heal.y);
						std::thread q(getItemSound);
						q.detach();
						ship.hp = ship.hp + 10;
						heal.status = 0;
					}
				}
			}
			//gold
			if (gold.status == 1)
			{
				if (gold.y >= 38)
				{
					clear_item(gold.x, gold.y);
					gold.status = 0;
				}
				else
				{
					if (frame % 30 == 0)
					{
						clear_item(gold.x, gold.y);
						draw_gold(gold.x, ++gold.y);
					}
					if (abs(gold.y - ship.y) < 4 && abs((ship.x + 6) - gold.x) < 7)
					{
						clear_item(gold.x, gold.y);
						std::thread q(getItemSound);
						q.detach();
						ship.status = 1;
						gold.status = 0;
						gold.time = frame;
					}
				}
			}

			//shipstatus
			if (ship.status==1)//gold
			{
				if (frame - gold.time > 800)
				{
					ship.status = 0;
					gold.time = 0;
				}
			}


			//if (score > changelevel) { level++; maxenemy++; changelevel += 100; }


			//changelevel
			if (score > changelevel)
			{
				level++;
				changelevel += 100;

				if (level == 2)
				{
					maxenemy = 3;
				}
				if (level == 3)
				{
					maxenemy2 = 1;
				}
				if (level == 4)
				{
					maxenemy = 3;
					maxenemy2 = 2;
				}
				if (level == 5)
				{
					maxenemy = 4;
					maxenemy2 = 4;
					maxmissile = 5;
				}
				if (level == 6)
				{
					maxenemy = 4;
					maxenemy2 = 4;
				}
				if (level == 7)
				{
					maxenemy = 5;
					maxenemy2 = 5;
				}
			}
			//gameover
			if (ship.hp <= 0)
			{
				clear_screen();
				frame = 0;
				page = 3;
			}

			if (frame % 50 == 0)
			{
				score++;
			}
			frame++;
			Sleep(10);
		}
		if (page == 2)//scoreboard
		{
			gotoxy(53, 12); printf("SCORE BOARD");
			gotoxy(48, 30); printf("PRESS E TO BACK TO MENU");
			scoreRead("score.txt");
			for (int i = 0; i < 5; i++)
			{
				gotoxy(50, 16 + (2*i));
				printf("%s", data[i].name);
				gotoxy(65, 16 + (2 * i));
				printf("%d", data[i].score);
			}	
		}
		if (page == 3)//gameover
		{
			scoreRead("score.txt");
			gotoxy(55, 14); gameover();
			gotoxy(55, 16); printf("ENTER YOUR NAME");
			gotoxy(55, 18);
			scanf_s("%19s", name,20);
			scoreAdd("score.txt", name,score);
			clear_screen();
			page = 0;
		}

	} while (ch != 'x');

	return 0;
}

