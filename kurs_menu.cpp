#include "windows.h"
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#define up 72
#define down 80
#define enter 13
#define esc 27

#define DX 20
#define DY 20

HDC hDC;
HWND hWnd;
RECT Rect;
HPEN hPen, hOldPen;
HBRUSH hBrush, hOldBrush;
HFONT hFont, hOldFont;
LOGFONT Lf = { 0 };

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

typedef struct {
	int x[4];
	COLORREF col;
	COLORREF BorderCol;
	char *text;
}Knopka;

typedef struct {
	Knopka *P;
	int n, m;
}Menu;

//graphics
void clearscreen(int r, int g, int b) {
	hPen = CreatePen(PS_SOLID, 4, RGB(r, g, b));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(RGB(r, g, b));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}
//graphics

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
	//установка цвета и стиля линий для построения функции (красный)
	//hPen = CreatePen(PS_SOLID, 4, color);
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
void chart_draw(int xn, int xk, float(*charts_function)(float)) {
	float b, maxy, miny, mx, my;
	int osx, osy;
	char ch;
	int scale = 1;
	COLORREF chart_color = RGB(0,0,0);
	int chart_type = PS_SOLID;
	HPEN chart_pen;
	
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
void knopka(Knopka *P) {
	hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(RGB(240, 240, 240));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	hFont = CreateFontIndirect(&Lf);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	Rectangle(hDC, P->x[0] - 4, P->x[1] - 4, P->x[0] + P->x[2] + 4,
		P->x[1] + P->x[3] + 4);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, P->x[0], P->x[1], P->x[0] + P->x[2] + 4,
		P->x[1] + P->x[3] + 4);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 4, P->col);//заливка фигуры
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(P->col);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, P->x[0], P->x[1], P->x[0] + P->x[2],
		P->x[1] + P->x[3]);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_DASH, 1, P->BorderCol);//бордер
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	SelectObject(hDC, hPen);
	SelectObject(hDC, GetStockObject(NULL_BRUSH));
	Rectangle(hDC, P->x[0], P->x[1], P->x[0] + P->x[2],
		P->x[1] + P->x[3]);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, P->col);
	TextOut(hDC, P->x[0] + 2, P->x[1] + 2, (LPCSTR)P->text,
		strlen(P->text));
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

Menu menu_init(char* punkt[], int l, int top, int width, int height, int n) {
	int i;
	Menu M;  M.n = n;  M.m = 0;
	M.P = new Knopka[n];
	for (i = 0; i < n; i++)
	{
		M.P[i].x[0] = l;
		if (i == 0)M.P[i].x[1] = top;
		else M.P[i].x[1] = M.P[i - 1].x[1] + M.P[i - 1].x[3] + 20;
		M.P[i].x[2] = width;
		M.P[i].x[3] = height;
		M.P[i].text = new char[strlen(punkt[i]) + 1];
		strcpy(M.P[i].text, punkt[i]);
	}
	return M;
}

void menu_del(Menu *M) {
	int i;
	for (i = 0; i < M->n; i++)
		delete[]M->P[i].text;
	delete[]M->P;
}

int menu_vert(Menu *M) {
	int i;
	char ch;

	do {
		for (i = 0; i < M->n; i++) {
			M->P[i].col = RGB(128, 128, 128);
			if (M->m == i) M->P[i].BorderCol = RGB(255, 0, 0); //активная
			else M->P[i].BorderCol = RGB(128, 128, 128);
			knopka(&(M->P[i]));
		}
		ch = _getch();
		switch (ch) {
		case up: if (M->m == 0) M->m = M->n - 1;
				 else (M->m)--;
				 break;
		case down: if (M->m == M->n - 1) M->m = 0;
				   else (M->m)++;
		}
	} while (ch != enter && ch != esc);
	if (ch == esc) M->m = M->n - 1;
	return M->m;
}

void panel(int panel_x, int panel_y, COLORREF color, char *text) {
	hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, (Rect.right / 2) - (panel_x / 2) - 5, (Rect.bottom / 2) - (panel_y / 2) - 5, (Rect.right / 2) + (panel_x / 2) + 5, (Rect.bottom / 2) + (panel_y / 2) + 5);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, (Rect.right / 2) - (panel_x / 2), (Rect.bottom / 2) - (panel_y / 2), (Rect.right / 2) + (panel_x / 2) + 5, (Rect.bottom / 2) + (panel_y / 2) + 5);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 4, color);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hBrush = CreateSolidBrush(color);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, (Rect.right / 2) - (panel_x / 2), (Rect.bottom / 2) - (panel_y / 2), (Rect.right / 2) + (panel_x / 2), (Rect.bottom / 2) + (panel_y / 2));
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	if (*text) {
		hFont = CreateFontIndirect(&Lf);
		hOldFont = (HFONT)SelectObject(hDC, hFont);
		SetTextColor(hDC, RGB(255, 0, 0));
		SetBkColor(hDC, color);
		RECT pos;
		pos.left = (Rect.right / 2) - (panel_x / 2);
		pos.right = (Rect.right / 2) + (panel_x / 2);
		pos.top = (Rect.bottom / 2) - (panel_y / 2);
		pos.bottom = (Rect.bottom / 2) + (panel_y / 2);
		DrawText(hDC, (LPCSTR)text, strlen(text), &pos, DT_CENTER);
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
}

void SetConsoleCyanColor() {
	SetConsoleTextAttribute(hout, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
}

void InputField(COORD coord, int size, int &data) {
	DWORD l;
	FillConsoleOutputAttribute(hout, BACKGROUND_BLUE | BACKGROUND_GREEN, 3, coord, &l);
	SetConsoleTextAttribute(hout, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN);
	SetConsoleCursorPosition(hout, coord);
	scanf("%d", &data);
	SetConsoleCyanColor();
}

void PrintLine(COORD &coord, char *str, bool cr) {
	SetConsoleCursorPosition(hout, coord);
	printf("%s", str);
	if(cr){ coord.Y++; }
}

void ArrayProcessing() {
	int *input_array = NULL, *tmp_array = NULL, *output_array = NULL;
	int Size_of_array, tmp_Size_of_array;
	int Select_variant;
	int Size_of_output_array = 0;
	int RangeBegin, RangeEnd;
	bool change = false;
	DWORD l;
	COORD point, init_coord, tmp_coord;
	point.X = 0; point.Y = 0;
	init_coord.X = 12;
	init_coord.Y = 5;

	SetConsoleOutputCP(1251);
	system("cls");
	FillConsoleOutputAttribute(hout, 0, 20000, point, &l); // очистка экрана
	clearscreen(0,0,255);

	panel(400, 500, RGB(0, 255, 255), "Обработка массива \n ---------------------------------------");


	//FillConsoleOutputAttribute(hout, 0, 2000, point, &l); // очистка экрана
	SetConsoleCyanColor();

	PrintLine(init_coord, "Размерность массива:", 1);
	tmp_coord = init_coord;
	tmp_coord.X += 21;
	tmp_coord.Y--;
	InputField(tmp_coord, 3, Size_of_array);
	input_array = new int[Size_of_array];
	tmp_array = new int[Size_of_array];
	output_array = new int[Size_of_array];
	init_coord.Y += 2;
	PrintLine(init_coord, "Формирование элементов массива:", 1);
	PrintLine(init_coord, "1) Случайным образом", 1);
	PrintLine(init_coord, "2) Ввод с клавиатуры", 1);
	PrintLine(init_coord, "3) Ввод из файла", 1);

	tmp_coord = init_coord;
	tmp_coord.X += 32;
	tmp_coord.Y -= 4;
	InputField(tmp_coord, 3, Select_variant);
	init_coord.Y += 2;
	//init_coord.Y++;
	switch (Select_variant) {
	case 2:
		PrintLine(init_coord, "Заполните массив:",1);
		for (int i = 0, yk = 0; i < Size_of_array; i++, yk += 3) {
			point = init_coord;
			point.X += yk; // сдваг следующего прямоугольника
			if (i % 2) {
				FillConsoleOutputAttribute(hout, BACKGROUND_BLUE | BACKGROUND_GREEN, 3, point, &l); //цвет фона фиолетовый прямоугольник размером в 3 знакоместа
				SetConsoleTextAttribute(hout, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN); // то что фореграунж это цвет текста(белый), остальное цвет фона
			} else {
				FillConsoleOutputAttribute(hout, BACKGROUND_RED | BACKGROUND_GREEN, 3, point, &l);
				SetConsoleTextAttribute(hout, 0 | BACKGROUND_RED | BACKGROUND_GREEN);
			}
			SetConsoleCursorPosition(hout, point);// следом за прямоугольником двигаем курсор
			scanf("%d", &input_array[i]);
		}
		break;
	case 1:
		PrintLine(init_coord, "Диапазон от:", 0);
		tmp_coord = init_coord;
		tmp_coord.X += 13;
		InputField(tmp_coord, 3, RangeBegin);
		tmp_coord.X += 3;
		PrintLine(tmp_coord, " до ", 0);
		tmp_coord.X += 5;
		InputField(tmp_coord, 3, RangeEnd);
		srand(time(NULL));
		for (int i = 0; i < Size_of_array; i++){
			input_array[i] = rand() % (RangeEnd + abs(RangeBegin)) + RangeBegin;
		}
		break;
	case 3:
		
		break;
	default:
		break;
	}
	init_coord.Y += 2;

	PrintLine(init_coord, "Обрабатываемый массив:", 1);

	SetConsoleCursorPosition(hout, init_coord);
	for (int i = 0; i < Size_of_array; i++) {
		printf("%d ", input_array[i]);
		tmp_array[i] = input_array[i];
	}
	tmp_Size_of_array = Size_of_array;

	for (int i = 0; i < Size_of_array - 1; i++) {
		change = false;
		for (int j = i + 1; j < Size_of_array; j++) {
			if (input_array[i] == input_array[j]) {
				output_array[Size_of_output_array++] = input_array[j];
				change = true;
				for (int k = j; k < Size_of_array; k++) {
					input_array[k] = input_array[k + 1];
				}
				j--;
				Size_of_array--;

			}
		}

		if (change) {
			output_array[Size_of_output_array++] = input_array[i];
			for (int k = i; k < Size_of_array; k++) {
				input_array[k] = input_array[k + 1];
			}
			i--;
			Size_of_array--;
		}
	}

	init_coord.Y += 2;
	PrintLine(init_coord, "Элементы которые повторялись более двух раз:", 1);

	for (int i = 0, yk = 0; i < tmp_Size_of_array; i++, yk += 3) {
		point = init_coord;
		point.X += yk; // сдваг следующего прямоугольника
		SetConsoleCursorPosition(hout, point);// следом за прямоугольником двигаем курсор
		for (int j = 0; j < Size_of_output_array; j++) {
			if (tmp_array[i] == output_array[j]) {
				FillConsoleOutputAttribute(hout, abs(tmp_array[i]) << 4, 3, point, &l);
				SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | abs(tmp_array[i]) << 4);
				printf("%d", tmp_array[i]);
				break;
			}
			if (j == Size_of_output_array - 1) {
				FillConsoleOutputAttribute(hout, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY, 3, point, &l);
				SetConsoleTextAttribute(hout, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY | 0);
				printf("%d", tmp_array[i]);
			}


		}



	}
	init_coord.Y += 2;
	SetConsoleCyanColor();
	PrintLine(init_coord, "Массив элементов которые повторялись более двух раз:", 1);
	SetConsoleCursorPosition(hout, init_coord);
	for (int i = 0; i < Size_of_output_array; i++) {
		printf("%d ", output_array[i]);
	}

	delete[] output_array;
	delete[] input_array;

}

void ChartProcessing() {
	float(*charts_function)(float);
	

	int size_x = 150;
	int size_y = 50;
	int panel_x = 400;
	int panel_y = 300;
	char *S[] = { "cosx/sqrt(x^2+1)","x^2/e^x-1","x*e^(-x)"};
	int m = 0;
	//do {
		clearscreen(0,0,255);
		panel(panel_x, panel_y, RGB(0, 128, 128), "");
		Menu M = menu_init(S, (Rect.right / 2) - (size_x / 2), (Rect.bottom / 2) - (size_y * 4 / 2) - 10 * 4, size_x, size_y, 3);// текст,лево,право,размер размер, колво пунктов
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