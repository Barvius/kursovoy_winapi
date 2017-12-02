#pragma once
void ArrayProcessing() {
	int *input_array = NULL, *tmp_array = NULL, *output_array = NULL;
	int Size_of_array, tmp_Size_of_array;
	int Select_variant;
	int Size_of_output_array = 0;
	int RangeBegin, RangeEnd;
	bool change = false;
	FILE *f;
	DWORD l;
	COORD point, init_coord, tmp_coord;
	point.X = 0; point.Y = 0;
	init_coord.X = 12;
	init_coord.Y = 5;
	SetConsoleOutputCP(1251);
	system("cls");
	FillConsoleOutputAttribute(hout, 0, 20000, point, &l);
	clearscreen(0, 0, 255);
	panel(400, 500, RGB(0, 255, 255), "Обработка массива \n ---------------------------------------");
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

	switch (Select_variant) {
	case 2:
		PrintLine(init_coord, "Заполните массив:", 1);
		for (int i = 0, yk = 0; i < Size_of_array; i++, yk += 3) {
			point = init_coord;
			point.X += yk;
			if (i % 2) {
				FillConsoleOutputAttribute(hout, BACKGROUND_BLUE | BACKGROUND_GREEN, 3, point, &l);
				SetConsoleTextAttribute(hout, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN);
			} else {
				FillConsoleOutputAttribute(hout, BACKGROUND_RED  | BACKGROUND_INTENSITY, 3, point, &l);
				SetConsoleTextAttribute(hout, 0 | BACKGROUND_RED  | BACKGROUND_INTENSITY);
			}
			SetConsoleCursorPosition(hout, point);
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
		for (int i = 0; i < Size_of_array; i++) {
			input_array[i] = rand() % (RangeEnd + abs(RangeBegin)) + RangeBegin;
		}
		break;
	case 3:
		if (!(f = fopen("arrayd.txt", "r"))) {
			tmp_coord.X = init_coord.X;
			tmp_coord.Y+=5;
			SetConsoleCursorPosition(hout, tmp_coord);
			printf("Файл не найден....\n");
			tmp_coord.Y++;
			SetConsoleCursorPosition(hout, tmp_coord);
			system("pause");
			exit(-1);
		} else {
			int i = 0;
			int n;
				while (fscanf(f, "%d\n", &input_array[i]) != EOF) {
					i++;
				}
				if (i < Size_of_array - 1) {
					for (; i < Size_of_array; i++) {
						input_array[i] = rand() % (10 + 1) + 1;
					}
				}
		}
		fclose(f);
		break;
	default:
		break;
	}
	init_coord.Y += 2;
	SetConsoleCyanColor();
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
		point.X += yk;
		SetConsoleCursorPosition(hout, point);
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