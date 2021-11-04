#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <thread>
#include <mutex>

std::mutex mtx;

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

void draw_ship(int x, int y)
{
	setcolor(2, 0);
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


void draw_heal(int x,int y)
{
	setcolor(2, 0);
	gotoxy(x, y); printf("(H)");
}

void clear_heal(int x, int y)
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

int xy[2];int count = 0;int around = 0;
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
				if (checkenemy != ' ')
				{
					count++;
				}
			}
		}
		if (count == 0)
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
			count = 0;
		}
		
	}
	
	
}

struct Bullet
{
	int status = 0;
	int x = 0, y = 0;
}bs[50];

struct Enemy
{
	int hp = 10;
	int x = 0, y = 0;
	int status = 0;
}enemy[10];

struct Enemy2
{
	int hp = 30;
	int x = 0, y = 0;
	int status = 0;
}enemy2[10];

struct Missile
{
	int hp = 10;
	int x = 0, y = 0;
	int dx = 0;
	int status = 0;
}missile[5];

int newex, newey;
int* rnewxy;
void rerandomenamyafterhit(int c) {
	enemy[c].status = 0;
	newex = 90 + (rand() % 17);
	newey = 5 + (rand() % 32);
	rnewxy = checkotherenemy(newex, newey);
	enemy[c].x = *(rnewxy + 0);
	enemy[c].y = *(rnewxy + 1);
	draw_enemy(enemy[c].x, enemy[c].y);
	enemy[c].status = 1;
	enemy[c].hp = 10;
}

void rerandomenamy2afterhit(int c) {
	enemy2[c].status = 0;
	newex = 90 + (rand() % 17);
	newey = 5 + (rand() % 32);
	rnewxy = checkotherenemy(newex, newey);
	enemy2[c].x = *(rnewxy + 0);
	enemy2[c].y = *(rnewxy + 1);
	draw_enemy2(enemy2[c].x, enemy2[c].y);
	enemy2[c].status = 1;
	enemy2[c].hp = 30;
}

int newmx, newmy;
int* rnewmxy;
void rerandommissileafterhit(int c) {
	missile[c].status = 0;
	newmx = 90 + (rand() % 17);
	newmy = 5 + (rand() % 32);
	rnewmxy = checkotherenemy(newmx, newmy);
	missile[c].x = *(rnewmxy + 0);
	missile[c].y = *(rnewmxy + 1);
	draw_missile(missile[c].x, missile[c].y);
	missile[c].status = 1;
	missile[c].hp = 10;
}

struct Heal
{
	int x = 5;
	int y = 3;
	int status = 0;

}heal;

unsigned long frame = 1;
int maxenemy = 2;
int maxenemy2 = 0;
int maxmissile = 3;
int changelevel = 100;

int level = 1;
int main()
{
	char ch = ' ';
	int x = 10, y = 20;
	int hp = 90;
	setcursor(0);
	int i;

	draw_ship(x, y);
	int current = 0;
	srand(time(NULL));


	int enemycount = 0;
	do {
		//สุ่มเกิดศัตรู
		for (i = 0; i < maxenemy; i++)
		{
			if (enemy[i].status == 0) 
			{
				enemy[i].x = 90 + (rand() % 17);
				enemy[i].y = 5 + (rand() % 32);
				checkotherenemy(enemy[i].x, enemy[i].y);
				draw_enemy(enemy[i].x, enemy[i].y);
				enemy[i].status = 1;
			}	
		}
		//สุ่มเกิด enemy2
		for (i = 0; i < maxenemy2; i++)
		{
			if (enemy2[i].status == 0)
			{
				enemy2[i].x = 90 + (rand() % 17);
				enemy2[i].y = 5 + (rand() % 32);
				checkotherenemy(enemy2[i].x, enemy2[i].y);
				draw_enemy2(enemy2[i].x, enemy2[i].y);
				enemy2[i].status = 1;
			}
		}
		//สุ่มเกิดmissile
		for (i = 0; i < maxmissile; i++)
		{
			if (missile[i].status == 0) 
			{
				missile[i].x = 90 + (rand() % 17);
				missile[i].y = 5 + (rand() % 32);
				checkotherenemy(missile[i].x, missile[i].y);
				draw_missile(missile[i].x, missile[i].y);
				missile[i].status = 1;
			}
		}

		print_score(score);
		print_hp(hp);
		print_level(level);
		print_frame(frame);
		setcolor(4, 0); gotoxy(70, 0); printf("count : %d", count);
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a')
			{
				if (x > 0)
				{
					erase_ship(x, y); draw_ship(--x, y);
				}
				else { draw_ship(x, y); }
			}
			if (ch == 'd')
			{
				if (x < 108)
				{
					erase_ship(x, y);
					draw_ship(++x, y);
				}
				else { draw_ship(x, y); }
			}
			if (ch == 's')
			{
				if (y < 37)
				{
					erase_ship(x, y);
					draw_ship(x, ++y);
				}
				else { draw_ship(x, y); }
			}
			if (ch == 'w')
			{
				if (y > 2)
				{
					erase_ship(x, y);
					draw_ship(x, --y);
				}
				else { draw_ship(x, y); }
			}
			if (ch == ' ' && bs[current].status == 0)
			{
				bs[current].status = 1;
				bs[current].x = x + 12;
				bs[current].y = y + 1;
				current = (current + 1) % 50;
			}
			fflush(stdin);
		}
		//bullet 

		for (int i = 0; i < 50; i++)
		{
			if (bs[i].status == 1)
			{
				clear_bullet(bs[i].x, bs[i].y);
				if (bs[i].x >= 116)
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
							enemycount += 1;
							clear_enemy(enemy[e].x, enemy[e].y);
							score += 10;
							rerandomenamyafterhit(e);
						}
					}
					for (int e = 0; e < maxenemy2; e++)//enemy2
					{
						if (abs(enemy2[e].y - bs[i].y) <= 1 && abs((enemy2[e].x + 5) - (bs[i].x + 4)) < 5)
						{
							clear_bullet(bs[i].x, bs[i].y);
							enemy2[e].hp -= 10;
							bs[i].status = 0;

						}
						if (enemy2[e].hp == 0)
						{
							enemycount += 1;
							clear_enemy(enemy2[e].x, enemy2[e].y);
							score += 10;
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
				if (enemy[e].x <= 0)//ชนขอบ
				{
					clear_enemy(enemy[e].x, enemy[e].y);
					rerandomenamyafterhit(e);
				}
				else
				{
					if (abs(enemy[e].y - y) <= 2 && abs((enemy[e].x) - (x + 11)) < 2)
					{
						erase_ship(x, y);
						Sleep(20);
						draw_ship(x, y);
						clear_enemy(enemy[e].x, enemy[e].y);
						rerandomenamyafterhit(e);
						hp = hp-10;
					}
					else
					{
						if (frame % 3 == 0) 
						{ clear_enemy(enemy[e].x, enemy[e].y); draw_enemy(--enemy[e].x, enemy[e].y); }
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
					rerandomenamy2afterhit(e);
				}
				else
				{
					if (abs(enemy2[e].y - y) <= 2 && abs((enemy2[e].x) - (x + 11)) < 2)
					{
						erase_ship(x, y);
						Sleep(20);
						draw_ship(x, y);
						clear_enemy(enemy2[e].x, enemy2[e].y);
						rerandomenamy2afterhit(e);
						hp = hp - 10;
					}
					else
					{
						if (frame % 3 == 0)
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
				if (missile[e].x <= 0)//ชนขอบ
				{
					clear_missile(missile[e].x, missile[e].y);
					rerandommissileafterhit(e);
				}
				else
				{
					if (abs(missile[e].y - y) <= 1 && abs((missile[e].x) - (x + 11)) < 2)
					{
						erase_ship(x, y);
						Sleep(20);
						draw_ship(x, y);
						clear_missile(missile[e].x, missile[e].y);
						rerandommissileafterhit(e);
						hp -= 10;
					}
					else
					{
						if (missile[e].y < y) 
						{
							if (frame % 7 == 0)
							{
								clear_missile(missile[e].x, missile[e].y);
								if (missile[e].dx < 3)
								{
									draw_missile(--missile[e].x, missile[e].y); missile[e].dx += 1;
								}
								else { draw_missile(--missile[e].x, ++missile[e].y); missile[e].dx = 0; }
							}
						}
						else if (missile[e].y > y)
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
		//item drop 
		if (frame % 1000 == 0 && heal.status == 0) 
		{
			heal.x = 40 + (rand() % 20);
			heal.y = 2 + (rand() % 3);
			draw_heal(heal.x, heal.y);
			heal.status = 1;
		}
		if (heal.status==1)
		{
			if (heal.y >= 38)
			{
				clear_heal(heal.x, heal.y);
				heal.status = 0;
			}
			else 
			{
				if (frame % 30 == 0)
				{
					clear_heal(heal.x, heal.y);
					draw_heal(heal.x, ++heal.y);
				}
				if (abs(heal.y - y) < 3  && abs((x + 6) - heal.x) < 6)
				{
					clear_heal(heal.x, heal.y);
					hp = hp + 10;
					heal.status = 0;
				}
			}
		}
		//if (score > changelevel) { level++; maxenemy++; changelevel += 100; }


		//changelevel
		if (enemycount == 5)
		{
			level++;
			enemycount = 0;
		}
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



		//gameover
		if (hp == 0)
		{
			erase_ship(x, y);
			for (i = 0; i < maxenemy; i++)
			{
				clear_enemy(enemy[i].x, enemy[i].y);
				enemy[i].status = 0;
			}
			for (i = 0; i < maxmissile; i++)
			{
				clear_missile(missile[i].x, missile[i].y);
				missile[i].status = 0;
			}
			clear_heal(heal.x, heal.y);
			heal.status = 0;
			gotoxy(55, 20);
			gameover();
		}

		if (frame % 50 == 0)
		{
			score++;
		}
		frame++;
		
		Sleep(10);

	} while (ch != 'x');

	return 0;
}

