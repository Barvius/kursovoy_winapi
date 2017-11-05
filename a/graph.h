#pragma once

HDC hDC;
HWND hWnd;
RECT Rect;
HPEN hPen, hOldPen;
HBRUSH hBrush, hOldBrush;
HFONT hFont, hOldFont;
LOGFONT Lf = { 0 };

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

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
	if (cr) { coord.Y++; }
}