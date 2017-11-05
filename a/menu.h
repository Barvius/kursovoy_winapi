#pragma once

#define up 72
#define down 80
#define enter 13
#define esc 27

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
