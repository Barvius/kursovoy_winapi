#pragma once
void FilesProcessing() {
	FILE *f;
	char b[100] = { NULL };
	char glasn[] = "аоиеЄэыую€";
	char *ykaz;
	char **text;
	int j=0, i,col_i=0;
	if (!(f = fopen("text.txt", "r"))) {
		printf("‘айл не найден....\n");
	} else {
		while (fgets(b, 100, f)) {
			col_i++;
		}
		char **text = new char*[col_i];
		fseek(f, 0L, SEEK_SET);
		COORD c;
		c.X = 2;
		c.Y = 5;
		panel(540, 500, RGB(0, 255, 255), "ќбработка файла");
		SetConsoleCyanColor();
		while (fgets(b, 100, f)) {
			text[j] = new char[strlen(b) + 1];
			strcpy(text[j], b);
			SetConsoleCursorPosition(hout, c);
			puts(text[j]);
			c.Y++;
			j++;	
		}
		fclose(f);
		
		
		
		char *tmp;
		int k;
		int len;
		for (i = 0; i < col_i; i++) {
			for (j = 0; j < strlen(glasn); j++) {
				for (k = 0; k < strlen(text[i]);k++) {
					if (text[i][k] == glasn[j]) {
						tmp = new char[strlen(text[i]) + 2];
						strcpy(tmp, text[i]);
						delete[] text[i];
						text[i] = new char[strlen(tmp)+2];
						strcpy(text[i], tmp);
						for (int a = strlen(text[i])+1; a > k+1;a--) {
							char tmm = text[i][a];
							text[i][a] = text[i][a-1];
							text[i][a - 1] = tmm;
						}
						text[i][k + 1] = ',';
						delete[] tmp;
					}
				}
				
			}
		}
		c.Y++;
		for (i = 0; i < col_i; i++) {
			SetConsoleCursorPosition(hout, c);
			puts(text[i]);
			c.Y++;
		}
		f = fopen("out.txt", "w+");// открываем файл в котором будут результаты работы программы
		for (i = 0; i < col_i; i++) {
			if (text[i][0]) {// поидеи можно и без него, пробуй
				//printf(text[i]);
				fputs(text[i], f);// построчно загон€ем массив в файл
			}
		}
		fclose(f);// закрываем 
		//panel(400, 500, RGB(0, 255, 255), text[0]);
	}
}