#include "windows.h"
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<locale.h>

#include"graph.h"
#include"menu.h"
#include"array.h"
#include"chart.h"
#include"file.h"


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	int size_x, size_y, panel_x, panel_y;
	size_x = 150;
	size_y = 50;
	panel_x = 400;
	panel_y = 500;
	hWnd = GetConsoleWindow();
	if (!hWnd) {
		printf("Can't get hWnd of console!\n");
		_getch();
		return -1;
	}
	MoveWindow(hWnd, 0, 0, 800, 600, TRUE);
	hDC = GetDC(hWnd);
	if (!hDC) {
		printf("Can't get device context!\n");
		_getch();
		return -2;
	}
	GetClientRect(hWnd, &Rect);
	char *S[] = { "Обработка массива","Работа с файлами","График функции","Выход " };
	int m = 0;
	DWORD l;
	COORD c;
	c.X = 0;
	c.Y = 0;
	do{
		SetConsoleCursorPosition(hout, c);
		clearscreen(0, 0, 255);
		panel(panel_x, panel_y, RGB(0, 128, 128), "");
		Menu M = menu_init(S, (Rect.right / 2) - (size_x / 2), (Rect.bottom / 2) - (size_y * 4 / 2) - 10 * 4, size_x, size_y, 4);// текст,лево,право,размер размер, колво пунктов
		m = menu_vert(&M);
		switch (m){
		case 0:
			ArrayProcessing();
			break;
		case 1:
			FilesProcessing();
			break;
		case 2:
			ChartProcessing();
			break;
		}
		_getch();
	} while (m != 3);
	ReleaseDC(hWnd, hDC);
	return 0;
}
