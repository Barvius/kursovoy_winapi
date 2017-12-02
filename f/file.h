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
		system("pause");
		exit(-1);
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
		char *tmp_l, *tmp_r;
		int k, s;
		int len;
		for (i = 0; i < col_i; i++) {
			for (j = 0; j < strlen(glasn); j++) {
				for (k = 0; k < strlen(text[i]);k++) {
					if (text[i][k] == glasn[j]) {
						size_t size = strlen(text[i]);
						tmp_l = new char[k + 2];
						tmp_r = new char[size-k+1];
						strncpy(tmp_l, text[i], k + 1);
						tmp_l[k+1] = '\0';

						s = 0;
						for (int a = k+1; a < size; a++) {
							tmp_r[s] = text[i][a];
							s++;
						}
						tmp_r[s] = '\0';

						delete[] text[i];
						text[i] = new char[size + 2]{'\0'};

						strcat(text[i], tmp_l);
						strcat(text[i], ",");
						strcat(text[i], tmp_r);
						
						
						delete[] tmp_l;
						delete[] tmp_r;
					}
				}
				
			}
		}
		f = fopen("out.txt", "w+");
		for (i = 0; i < col_i; i++) {
			if (text[i][0]) {
				fputs(text[i], f);
			}
		}
		fseek(f,SEEK_SET,0);
		c.Y++;
		while (fgets(b, 100, f)) {
			SetConsoleCursorPosition(hout, c);
			puts(b);
			c.Y++;
		}
		fclose(f);
	}
}