#include "windows.h"
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"graph.h"
#include"menu.h"
#include"array.h"

#define DX 20
#define DY 20

//charts
//
float f_charts_1(float x) {
	return cos(x)/sqrt(powf(x,2)+1);
}
float f_charts_2(float x) {
	return powf(x, 2) / (exp(x) - 1);
}
float f_charts_3(float x) {
	return x*exp(-x);
}
//
void mashtab(float xn, float xk, float *mx, float *my, float *miny, float *maxy, float(*func)(float)) {
	float x, y, dx;
	x = xn;
	dx = (xk - xn) / 100;
	*maxy = -1000;
	*miny = 1000;
	while (x <= xk) {
		y = func(x);
		if (y>*maxy) *maxy = y;
		if (y<*miny) *miny = y;
		x = x + dx;
	}
	*my = (Rect.bottom - 2 * DY) / (*maxy - *miny);
	*mx = (Rect.right - 2 * DX) / (xk - xn);
}

void osi(float minx, float maxx, float miny, float maxy, float mx, float my, int *osx, int *osy) {
	if (minx*maxx<0) *osy = fabs(minx)*mx + DX;
	else if (minx >= 0) *osy = DX;
	else *osy = Rect.right - DX;
	if (miny*maxy<0) *osx = Rect.bottom - fabs(miny)*my - DY;
	else if (miny >= 0) *osx = Rect.bottom - DY;
	else *osx = DY;
	hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	MoveToEx(hDC, DX, *osx, NULL);
	LineTo(hDC, Rect.right - DX, *osx);
	MoveToEx(hDC, *osy, DY, NULL);
	LineTo(hDC, *osy, Rect.bottom - DY);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void setka(int osx, int osy, float minx, float maxx, float miny, float maxy, float mx, float my) {
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	Lf.lfHeight = 12;
	Lf.lfWeight = 900;
	Lf.lfWidth = 8;
	CHAR S[20];
	strcpy((char*)Lf.lfFaceName, "Times New Roman");
	hFont = CreateFontIndirect(&Lf);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, RGB(255, 255, 255));
	TextOut(hDC, osy - 20, Rect.top + DY - 8, (LPCSTR)"Y", 1);
	TextOut(hDC, Rect.right - DX + 2, osx - 20, (LPCSTR)"X", 1);
	//построение сетки
	float dx, x, y;
	int xt, yt;
	RECT d;
	dx = (maxx - minx) / 20;
	for (x = minx; x <= maxx; x += dx) {
		xt = DX + mx*(x - minx);
		MoveToEx(hDC, xt, Rect.top + DY, NULL);
		LineTo(hDC, xt, Rect.bottom - DY);
		sprintf(S, "%.1f", x);
		TextOut(hDC, xt, osx + 8, (LPCSTR)S, strlen(S));
	}
	float dy = (maxy - miny) / 20;
	for (y = miny; y<maxy + dy; y += dy) {
		yt = Rect.bottom - my*(y - miny) - DY;
		MoveToEx(hDC, Rect.left + DX, yt, NULL);
		LineTo(hDC, Rect.right + DX, yt);
		sprintf(S, "%.1f", y);
		TextOut(hDC, osy + 5, yt, (LPCSTR)S, strlen(S));
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void grafik(HPEN hPen, float minx, float maxx, float miny, float mx, float my, float(*func)(float)) {
	float x, y, dx;
	int xt, yt;
	dx = (maxx - minx) / 200;
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	//расчет координат начальной точки функции	
	x = minx;
	y = func(x);
	xt = Rect.left + (x - minx)*mx + DX;
	yt = Rect.bottom - (y - miny)*my - DY;
	MoveToEx(hDC, xt, yt, NULL);
	//построение графика
	do {
		x = x + dx;
		y = func(x);

		xt = Rect.left + (x - minx)*mx + DX;
		yt = Rect.bottom - (y - miny)*my - DY;
		LineTo(hDC, xt, yt);
	} while (x<maxx - dx);
	//printf("%d %d\n", xt,yt);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void chart_draw(float xn, float xk, float(*charts_function)(float)) {
	float b, maxy, miny, mx, my;
	int osx, osy;
	char ch;
	int scale = 1;
	COLORREF chart_color = RGB(0,0,0);
	int chart_type = PS_SOLID;
	HPEN chart_pen;

	DWORD l;
	COORD point, init_coord;
	point.X = 0; point.Y = 0;
	init_coord.X = 12;
	init_coord.Y = 5;

	system("cls");
	FillConsoleOutputAttribute(hout, 0, 20000, point, &l); // очистка экрана
	clearscreen(0, 0, 255);
	panel(400, 500, RGB(0, 255, 255), "График функции \n ---------------------------------------");
	SetConsoleCyanColor();
	char S[20];
	float x, y, dx;
	x = xn;
	dx = (xk - xn) / 30;
	PrintLine(init_coord, "+---------+---------+", 1);
	PrintLine(init_coord, "+    X    +    Y    +", 1);
	PrintLine(init_coord, "+---------+---------+", 1);
	while (x <= xk) {
		y = charts_function(x);
		sprintf(S,"|%-8.4f | %-8.4f|",x,y);

		PrintLine(init_coord,S,1);
		x = x + dx;
	}
	PrintLine(init_coord, "+---------+---------+", 1);
	_getch();
	do {
		chart_pen = CreatePen(chart_type, 1, chart_color);
		clearscreen(255, 255, 255);
		mashtab(xn, xk, &mx, &my, &miny, &maxy, charts_function);
		osi(xn, xk, miny, maxy, mx, my, &osx, &osy);
		setka(osx, osy, xn, xk, miny, maxy, mx, my);
		grafik(chart_pen, xn, xk, miny, mx, my, charts_function);

		ch = _getch();
		switch (ch) {
		case 61: //+
			if ((xk - xn) > 2.0) {
				xn += scale;
				xk -= scale;
			}
			break;
		case 45: //-
			xn -= scale;
			xk += scale;
			break;
		case 75: // l
			xn -= scale;
			xk -= scale;
			break;
		case 77: //r
			xn += scale;
			xk += scale;
			break;
		case 'c': //r
			chart_color = RGB(rand() % 255, rand() % 255, rand() % 255);
			break;
		case 't':
			int types[] = { PS_SOLID ,PS_DASH ,PS_DOT ,PS_DASHDOT ,PS_DASHDOTDOT };
			chart_type = types[rand()%5];
			break;
		}

	} while (ch != 27);
	
}
//charts
// menu





void ChartProcessing() {
	float(*charts_function)(float);
	

	int size_x = 150;
	int size_y = 50;
	int panel_x = 400;
	int panel_y = 300;
	char *S[] = { "cosx/sqrt(x^2+1)","x^2/e^x-1","x*e^(-x)"};
	int m = 0;
	//do {
		//clearscreen(0,0,255);
		//panel(panel_x, panel_y, RGB(0, 128, 128), "");
		Menu M = menu_init(S, (Rect.right / 2) - (size_x / 2)+160, (Rect.bottom / 2) - (size_y * 4 / 2) - 10 * 4 +100, size_x, size_y, 3);// текст,лево,право,размер размер, колво пунктов
		m = menu_vert(&M);
		

		switch (m) {
		case 0:
			charts_function = f_charts_1;
			break;
		case 1:
			charts_function = f_charts_2;
			break;
		case 2:
			charts_function = f_charts_3;
			break;
		}
	//} while (m != 3);

	chart_draw(-4, 4, charts_function);
	
}

int main() {

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
	char *S[] = { "Обработка массива","График функции","Работа с файлами ","Выход " };
	int m = 0;
	do
	{
		clearscreen(0,0,255);
		panel(panel_x, panel_y, RGB(0, 128, 128), "");
		Menu M = menu_init(S, (Rect.right / 2) - (size_x / 2), (Rect.bottom / 2) - (size_y * 4 / 2) - 10 * 4, size_x, size_y, 4);// текст,лево,право,размер размер, колво пунктов
		m = menu_vert(&M);


		switch (m){
		case 0:
			ArrayProcessing();
			_getch();
			break;
		case 1:
			ChartProcessing();
			_getch();
			break;
		case 2:
			break;
		}
	} while (m != 3);
	ReleaseDC(hWnd, hDC);
	return 0;
}
