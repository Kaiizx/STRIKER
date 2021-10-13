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

int hp = 30;
void print_hp(int x)
{
	setcolor(4, 0);
	gotoxy(0, 0); printf("HP : %d", x);
}

void gameover() {
	setcolor(4, 0);
	printf("GAME OVER");
}

int xy[2];
int* checkotherenemy(int x, int y)
{
	int around = 0;
	int count = 0;
	while (around == 0)
	{
		int endx = x + 10;
		int endy = y + 7;
		for (int i = x - 10; i <= endx; i++)
		{
			for (int j = y - 7; j <= endy; j++)
			{
				//char checkenemy = cursor(x,y);
				char checkenemy = attribute(x, y);
				if (//checkenemy == '_'|| checkenemy == '\\'|| checkenemy == '*'|| checkenemy == '='|| checkenemy == '|'|| checkenemy == '/'
					checkenemy == 4)
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
		}
	}
}

struct Bullet
{
	int status = 0;
	int x = 0, y = 0;
}bs[99];

struct Enemy
{
	int hp = 10;
	int x = 0, y = 0;
	int status = 0;
}enemy[2];;

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
}


int main()
{
	char ch = ' ';
	int x = 10, y = 20;
	setcursor(0);
	int i;

	draw_ship(x, y);
	int current = 0;
	srand(time(NULL));
	for (i = 0; i < 2; i++)
	{
		enemy[i].x = 90 + (rand() % 17);
		enemy[i].y = 5 + (rand() % 32);
		checkotherenemy(enemy[i].x, enemy[i].y);
		draw_enemy(enemy[i].x, enemy[i].y);
		enemy[i].status = 1;
	}
	do {
		print_score(score);
		print_hp(hp);
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
				if (y > 0)
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
				bs[current].y = y + 2;
				current = (current + 1) % 99;
			}
			fflush(stdin);
		}
		//bullet agjkajgkjl;

		for (int i = 0; i < 99; i++)
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
					for (int e = 0; e < 2; e++)
					{
						if (abs(enemy[e].y - bs[i].y) <= 1 && abs((enemy[e].x + 5) - (bs[i].x + 4)) < 5)
						{
							clear_bullet(bs[i].x, bs[i].y);
							clear_enemy(enemy[e].x, enemy[e].y);
							bs[i].status = 0;
							score += 10;
							rerandomenamyafterhit(e);
						}
					}
				}

				/*clear_bullet(bs[i].x, bs[i].y);
				char checkenemy = attribute(bs[i].x + 5, bs[i].y);
				gotoxy(3, 2); printf("%d", checkenemy);
				if (bs[i].x >= 116)
				{
					bs[i].status = 0;
				}
				else
				{
					if (checkenemy == 4)
					{
						bs[i].status = 0;
						clear_bullet(bs[i].x, bs[i].y);
						clear_enemy_hit_bullet(bs[i].x , bs[i].y);

						/*if (bs[i].x + 5 == enemy[0].x)
						{
							rerandomenamyafterhit(0);
							score += 10;
						}
						if (bs[i].x + 5 == enemy[1].x)
						{
							rerandomenamyafterhit(1);
							score += 10;
						}*/

			}


		}

		//enemy_move

		for (int e = 0; e < 2; e++)
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
						hp -= 10;
					}
					else
					{
						clear_enemy(enemy[e].x, enemy[e].y);
						draw_enemy(--enemy[e].x, enemy[e].y);
						Sleep(30);
					}
				}
			}
		}

		if (hp == 0)
		{
			erase_ship(x, y);
			for (i = 0; i < 2; i++)
			{
				clear_enemy(enemy[i].x, enemy[i].y);
				enemy[i].status = 0;
			}
			gotoxy(55, 20);
			gameover();
		}


		Sleep(50);

	} while (ch != 'x');

	return 0;
}

