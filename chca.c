/*      _____
 *  |  /-----\  |
 *  |\|-------|/|
 *   \|-------|/
 *    /\-----/\
 *   | /\___/\ |
 *    |  /W\  |
 *  /===^   ^===\
 * /|   ch ca   |\
 * V/  HdiSSdH  \V
 * (             )
 * 
 * chca - simple tool for working with very primitive knowledge bases.
 * other information in README.chca
 *
 * Author:
 *          Jerzy Pavka
 *          mail jerzypavka@gmail.com
 *          site https://irrumator228.github.io/
 */

#define VERSION "chca-0.0.0.1-egg (c) 2023 Jerzy Pavka\n"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>

#define KMAG "\x1B[35m"
#define KNRM "\033[0m"

struct stack {
	char stck[100][100];
	unsigned int size;
	char str[100];
	unsigned int len[100];
	char type[100][5];

	//char f[100][100];
	//char s[100][1000];
	//unsigned int i[1000];
	//unsigned int j[1000];
	//unsigned int size;
};

struct cur {
	unsigned int i;
	unsigned int j;
};

struct fstck {
	char query[100][100];
	char f[100][100];
	char s[100][1000];
	unsigned int i[1000];
	unsigned int j[1000];
	unsigned int je[1000];
	unsigned int size;
};

void printi (char *s, unsigned int j) {
	if (s[strlen(s) - 1] == '\n') s[strlen(s) - 1] = ' ';
 	if (strlen(s) > 60 && j > 20) {
		printf("  ");
		for (unsigned int i = j - 20; i < j - 1; i++) {
			printf("%c", s[i]);
		}
		printf("%s -> %s", KMAG, KNRM);
		for (unsigned int i = j - 1; i < strlen(s) && i < j + 40; i++) {
			printf("%c", s[i]);
		}
		printf("\n");
	}
	else {
		printf("  ");
		for (unsigned int i = 0; i < j - 1; i++) {
			printf("%c", s[i]);
		}
		printf("%s -> %s", KMAG, KNRM);
		for (unsigned int i = j - 1; i < strlen(s) && i < j + 40; i++) {
			printf("%c", s[i]);
		}
		printf("\n");
	}
}

void merror (unsigned int n, char *s, unsigned int  i, unsigned int j) {
	char *err_s;
	switch (n) {
	case 1:
		err_s = "path not specified";
		break;
	case 2:
		err_s = "command not found";
		break;
	case 3:
		err_s = "open file";
		break;
	case 4:
		err_s = "close file";
		break;
	case 5:
		err_s = "unknown operator";
		break;
	case 6:
		err_s = "extra closing parenthesis";
		break;
	case 7:
		err_s = "unknown character";
		break;
	case 8:
		err_s = "extra opening parenthesis";
		break;
	case 9:
		err_s = "it's no such file or directory";
		break;
	case 10:
		err_s = "error in chca code";
		break;
	default:
		err_s = "unknown error";
		break;			
	}
	if ((i == 0) && (j == 0)) {
		printf("%s\n", s);
		printf("\n%u error. %s.\n", n, err_s);
	}
	else if (i == 0) {
		printi(s,j);
		printf("%u error. %s:%u\n", n, err_s, j);
	}
	else {
		printi(s,j);
		printf("%u error. %s. %s:%u:%u\n", n, err_s, s, i, j);
	}
	exit(EXIT_FAILURE);
}

void printer (unsigned int code, char *s, char *f, char *q, unsigned int i, unsigned int j) {
	switch (code) {
	case 1: //вывод вхождения из файла
		printi(s,j);
		printf("(%s) %s:%u:%u\n", q, f, i, j);
		break;
	case 2: //вывод отсутствия вхождения
		printf("  no entries found\n");
		printf("!(%s) %s\n", q, f);
		break;
	default: //вывод дефолта.
		printf("  chca");
		break;
	}
}

struct fstck find (char *s, char *f) {
	if ( f == NULL) merror(1, " ", 0, 0);
	
	char f_s[1000];
	
	FILE *file;
	file = fopen(f,"rt"); //только для чтения
	if (file == NULL) merror(3, f, 0, 0);
	
	int done = -1;
	
	struct fstck fstck1;
	
	unsigned int k = 0;

	for (unsigned int i = 0; fgets(f_s, 1000, file) != NULL; i++) {

		if (f_s[0] != ' ' || f_s[0] != ' ') {

			for (unsigned int j = 0; j < strlen(f_s); j++) {
				switch(f_s[j]) {
				case ' ':
					break;
				case '\n':
					break;
				case '\0':
					break;
				case '/':
					break;
				case '(':
					break;
				case ')':
					break;
				case '.':
					break;
				case '#':
					break;
				case ':':
					break;
				case '{':
					break;
				case '}':
					break;
				case '|':
					break;
				case '[':
					break;
				case ']':
					break;
				case '\'':
					break;
				case '"':
					break;
				default:
					break;
				}

				if (f_s[j] == s[0]) {
				for (unsigned int k = 0; k < strlen(s) && done < 0 ; k++) {
					if (f_s[j + k] == s[k]) done = -1;
					else done = 1;
				}
				if (done == -1) { 
					strcpy(fstck1.f[k],f);
					strcpy(fstck1.query[k], s);
					strcpy(fstck1.s[k], f_s);
					fstck1.i[k] = i + 1;
					fstck1.j[k] = j + 1;
					fstck1.size = k+1; 
					k++; 
				}
				else done = -1;
				}
			}
		}
	}
	fclose(file);
	if (k != 0) return fstck1;
	else { strcpy(fstck1.f[0], f); strcpy(fstck1.query[0], s); strcpy(fstck1.s[0], " "); fstck1.i[0] = 0, fstck1.j[0] = 0; fstck1.je[0] = 0; fstck1.size = 1; return fstck1;}
}

int listdir (char *s) {
	DIR *d;
	struct dirent *dir;
	d = opendir(s);
	
	if (!d) merror(1, s, 0, 0);
	
	//if (dir->d_type == DT_REG) printf("%s\n", dir->d_name);

	unsigned int f_i = 0;

	if (d) {
		while ((dir = readdir(d)) != NULL) { 	
			if ((strcmp(dir->d_name,".") > 0) && (strcmp(dir->d_name,"..") > 0)) {
				
				//stack1.

				printf("%s\n", dir->d_name);
			}
		}
		closedir(d);
	}
	return 0;
}

struct stack lexer (char *s, struct stack stck1) {

	

	struct stack stck2;
	
	unsigned int l = 0;
	struct cur cr = { 0 , 0 };

	char *s2 = "._/~-abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРCТУФХЦЧШЩЪЫЬЭЮЯ";
	int done = -1;

	for (unsigned int i = 0; i < stck1.size; i++) {

		//printf("%s", stck1.stck[i]);//========================
		
		for (unsigned int j = 0; j < strlen(stck1.stck[i]); j++ ) {
			//printf("%c\n", stck1.stck[i][j]);//========================
			switch (stck1.stck[i][j]) {
			case '\n':
			case '\0':

				done = 1;
				break;
			case '-':
			case '.':
			case '/':
				/*if (stck1.stck) {
					done = 1;
					if (cr.j != 0) cr.i++;
					stck1.stck[cr.i][0] = s[i];
					cr.i++;
					cr.j = 0;
				}*/
				break;
			case '(':
			case '{':
			case '[':
				//printf("%c\n", stck2.stck[cr.i][cr.j]);//========================
				l++;
				done = 1;
				if (cr.j != 0) cr.i++;
				stck2.stck[cr.i][0] = s[j];
				//printf("%c\n", stck2.stck[cr.i][cr.j]);//========================
				cr.i++;
				cr.j = 0;
				break;
			case ')':
			case '}':
			case ']':
				l--;
				done = 1;
				if (cr.j != 0) cr.i++;
				stck2.stck[cr.i][0] = s[j];
				cr.i++;
				cr.j = 0;
				break;
			case ' ':
				//stck2.stck[cr.j][0] = s[j];
				cr.j = 0;
				done = 1;
				break;
			case '&': 
			case '^': 
			case '!': 
			case '|':
			case '\\':
			case '?':
			case '#':
			case ',':
			case '%':
			case '+':
			case '=':
			case '@':
				done = 1;
				if (cr.j != 0) cr.i++;
				stck2.stck[cr.i][0] = s[j];
				cr.i++;
				cr.j = 0;
				break;
			}
			for (unsigned int k = 0; (k < strlen(s2) && done < 0); k++) {
				if (stck1.stck[i][j] == s2[k]) {
					done = 1;
					stck2.stck[cr.i][cr.j] = stck1.stck[i][j];
					cr.j++;
				}
			}
			if (done < 0) merror(7, s, 0, j);
			done = -1;
		}
	}
	stck2.size = cr.i + 1;
	//=========================================
	stck2.len[0] = 0;
	for (unsigned int i = 1; i < stck2.size; i++) {
		stck2.len[i] = stck2.len[i-1] + strlen(stck2.stck[i]);
	}
	//=========================================
	if (l != 0) { merror(8, s, 0, stck2.len[stck2.size-1]); }

	return stck2;
}

struct stack parser (struct stack stck1) {
	for (unsigned int i = 0; i < stck1.size; i++) {
		//printf("%s\n", stck1.stck[i]);//=============================
		switch (stck1.stck[i][0]) {
		case '(':
		case ')':
			break;
		case 'r':
			printf("aboba = '%c' ", stck1.stck[i][0]); stck1.type[i][0] = 'p';
			break;
		case '?':
		case '%':
			//stck1.type[i][0] = 'l';
			break;
		case '&':
		case '|':
		case '^':
		case '!':
			stck1.type[i][0] = 'o';
			break;
		case '1':
		case '0':
			stck1.type[i][0] = 'l';
			break;
		case '~':
		case '/':
			if (opendir(stck1.stck[i]) != NULL) { stck1.type[i][0] = 'd'; }
			else if (fopen(stck1.stck[i],"rt") != NULL) { stck1.type[i][0] = 'f'; }
			else merror(9, stck1.stck[i], 0, 0);
			break;
		default:
			stck1.type[i][0] = 'q';
			break;
		}
	}
	return stck1;
}

void runer (struct stack stck1) {
	struct fstck fstck1;

	for (unsigned int i = 0; i < stck1.size; i++) {

		/*if (stck1.type[i][0] == 'p') {

		}*/

		if (stck1.type[i][0] == 'q') {

			for (unsigned int j = 0; j < stck1.size; j++) {

				if (stck1.type[j][0] == 'f') {
					struct fstck fstck1 = find(stck1.stck[i], stck1.stck[j]);
					
					if (fstck1.i[0] != 0) {
						for (unsigned int j = 0; j < fstck1.size; j++) {
							printer(1, fstck1.s[j], fstck1.f[j], fstck1.query[j], fstck1.i[j], fstck1.j[j]);
						}
					}
					else printer(2, " ", fstck1.f[0], fstck1.query[0], 0, 0);
					
				}
			}
		}
	}
}

int logic (int argc, int argv[], char c) {
	switch (c) {
	case '&':
		//for (unsigned int i = 2)
		return (argv[1] && argv[2]);
		break;
	case '|':
		return (argv[1] || argv[2]);
		break;	
	case '!':
		return (!argv[1]);
		break;
	case '^':
		return (argv[1] ^ argv[2]);
		break;
	default:
		merror(5," ",0,0);
		break;
	}
}

void run(char *s) {
	struct stack stck1;
	strcpy(stck1.stck[0], s);
	stck1.size = 1;

	runer(parser(lexer(s, stck1)));
}

void runfromfile(char *f) {
	if ( f == NULL) merror(1, " ", 0, 0);
	char f_s[1000];
	FILE *file;
	file = fopen(f,"rt"); //только для чтения
	if (file == NULL) merror(3, f, 0, 0);

	for (unsigned int i = 0; fgets(f_s, 1000, file) != NULL; i++) {
		if ((f_s[0] != ' ' || f_s[1] != ' ') && f_s[0] != '\n') {
			run(f_s);
		}
	}
	fclose(file);
}

int main (int argc, char *argv[]) {
	setlocale(LC_CTYPE, "Russian.UTF-8"); /*change your language*/
	if (argc > 1) {
		
		/*
		unsigned int j = 0;
		for (unsigned int i = 0; i < argc; i++) {
			j += strlen(argv[j]);
		}

		char s[j];

		for (unsigned int i = 0; i < argc; i++) {
			j += strlen(argv[j]);
		}*/

		switch (argv[1][0]) {
		case '(':
			run(argv[1]);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			printf("commands:\nh - help;\nv - version;\nr /path/ - run from file;\nother commands and tutorial in file README.chca\n");
			exit(EXIT_SUCCESS);
			break;
		case 'v':
			printf("%s\n", VERSION);
			exit(EXIT_SUCCESS);
			break;
		case 'r':
			runfromfile(argv[2]);
			exit(EXIT_SUCCESS);
			break;
		default:
			printf("try h for help.\n");
			exit(EXIT_SUCCESS);
			break;
		}
		return 0;
	}
	else {
		printf("try h for help.\n");
		for(;;) {
			printf(">");
			char s[100];
			scanf("%s", s);
			switch(s[0]) {
			case '(':
				run(s);
				break;
			case 'r':
				runfromfile(s);
				break;
			case 'h':
				printf("commands:\nh - help;\nv - version;\nr /path/ - run from file;\nq - quit;\nother commands and tutorial in file README.chca;\n");
				break;
			case 'v':
				printf(VERSION);
				break;
			case 'q':
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("try h for help.\n");
				break;
			}
		}

	} 
}