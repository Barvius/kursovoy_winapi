
#include "stdafx.h"
#include "windows.h"
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>


int main(){
	DWORD l;
	COORD point;
	point.X = 0; point.Y = 0;
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	
	SetConsoleOutputCP(1251);
	
	FillConsoleOutputAttribute(hout, 0, 2000, point, &l); // очистка экрана
	int yk = 0;
	int arr[10];
	for (int i = 0; i < 10; i++,yk+=3) {
		point.X = yk; // сдваг следующего прямоугольника
		if (i%2) {
			FillConsoleOutputAttribute(hout, BACKGROUND_BLUE | BACKGROUND_RED, 3, point, &l); //цвет фона фиолетовый прямоугольник размером в 3 знакоместа
			SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED); // то что фореграунж это цвет текста(белый), остальное цвет фона
		} else {
			FillConsoleOutputAttribute(hout, BACKGROUND_BLUE , 3, point, &l);
			SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY|BACKGROUND_BLUE );
		}
		SetConsoleCursorPosition(hout, point);// следом за прямоугольником двигаем курсор
		scanf("%d", &arr[i]);
	}
	SetConsoleTextAttribute(hout, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
	
	for (size_t i = 0; i < 10; i++){
		printf("%d ",arr[i]);
	}
	//char c;
	//ReadFile(hin, &c, 1, &l, NULL);
	system("pause");
	return 0;
}
