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

#define VERSION "chca-0.0.0.2-egg (c) 2023 Jerzy Pavka\n"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define PRINT_BUF_SIZE 60
#define MIN_PRINT_BUF_SIZE 20
#define MIN_STR_SIZE 10
#define NORM_STR_SIZE 100
#define MAX_STR_SIZE 1000

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#if __linux__
#endif

struct stack {
	char stck[NORM_STR_SIZE][NORM_STR_SIZE];
	unsigned int len[NORM_STR_SIZE];
	unsigned int size;
	char type[NORM_STR_SIZE][5];

	//char f[NORM_STR_SIZE][NORM_STR_SIZE];
	//char s[NORM_STR_SIZE][MAX_STR_SIZE];
	//unsigned int i[MAX_STR_SIZE];
	//unsigned int j[MAX_STR_SIZE];
	//unsigned int size;
};

struct cur {
	unsigned int i;
	unsigned int j;
};

/*
typedef struct cur {
	unsigned int i;
	unsigned int j;
} cur;
*/

struct fstck {
	char query[NORM_STR_SIZE][NORM_STR_SIZE];
	char f[NORM_STR_SIZE][NORM_STR_SIZE];
	char s[NORM_STR_SIZE][MAX_STR_SIZE];
	unsigned int i[MAX_STR_SIZE];
	unsigned int j[MAX_STR_SIZE];
	unsigned int je[MAX_STR_SIZE];
	unsigned int size;
};

typedef struct file_entry_list {
	//int status; //1 - entry not found, 0 - entry exists
	char file[NORM_STR_SIZE];
	char entry[PRINT_BUF_SIZE];
	unsigned int i;
	unsigned int j;
} file_entry_list;

typedef struct file_entry_list_head {
	struct file_entry_list *head;
} file_entry_list_head;

typedef struct lexem_list {
	char lexem[NORM_STR_SIZE];
	char type[3];
	file_entry_list_head *head;
	struct lexem_list *next;
} lexem_list;

typedef struct lexem_list_head {
	struct lexem_list *head;
} lexem_list_head;

/*
void remove_st(lexem_list_head *l, lexem_list *target) {
    lexem_list **p = &l->head;
    while ((*p) != target) {
        p = &(*p)->next;
    }
    *p = target->next;
}*/

void push_st(lexem_list_head *l, lexem_list *target) {

}

void new_st(lexem_list_head *l, lexem_list *target) {
	
}

void printi (char *s, unsigned int j) {
	if (s[strlen(s) - 1] == '\n') s[strlen(s) - 1] = ' ';
 	if (strlen(s) > PRINT_BUF_SIZE && j > MIN_PRINT_BUF_SIZE) {
		printf("  ");
		for (unsigned int i = j - MIN_PRINT_BUF_SIZE; i < j - 1; i++) {
			printf("%c", s[i]);
		}
		printf("%s -> %s", KMAG, KNRM);
		for (unsigned int i = j - 1; i < strlen(s) && i < j + PRINT_BUF_SIZE-MIN_PRINT_BUF_SIZE; i++) {
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
		for (unsigned int i = j - 1; i < strlen(s) && i < j + PRINT_BUF_SIZE-MIN_PRINT_BUF_SIZE; i++) {
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
		err_s = "extra closing/opening parenthesis";
		break;
	case 7:
		err_s = "unknown character";
		break;
	case 8:
		err_s = " error code not reserved";
		break;
	case 9:
		err_s = "it's no such file or directory";
		break;
	case 10:
		err_s = "error in chca code";
		break;
	case 11:
		err_s = "buffer overflow";
		break;
	default:
		err_s = "unknown error";
		break;
	}
	//?????????? ?????? ??????????, ???????????? ?? ???????????? ?????????????? 
	if ((i == 0) && (j == 0)) {
		printf("%s\n", s);
		printf("%u error. %s.\n", n, err_s);
	}
	//?????????? ???????????? ?? ???????????? ??????????????
	else if (i == 0) {
		printi(s,j);
		printf("%u error. %s:%u\n", n, err_s, j);
	}
	//?????????? ??????????, ???????????? ?? ???????????? ??????????????
	else {
		printi(s,j);
		printf("%u error. %s. %s:%u:%u\n", n, err_s, s, i, j);
	}
	exit(EXIT_FAILURE);
}

void printer (unsigned int code, char *s, char *f, char *q, unsigned int i, unsigned int j) {
	switch (code) {
	case 1: //?????????? ?????????????????? ???? ??????????
		printi(s,j);
		printf("(%s) %s:%u:%u\n", q, f, i, j);
		break;
	case 2: //?????????? ???????????????????? ??????????????????
		printf("  no entries found\n");
		printf("!(%s) %s\n", q, f);
		break;
	case 3: //?????????? ?????????????????? ???? ?????????? ?? ?????????????????????????? clear
		printf("(%s) %s:%u:%u\n", q, f, i, j);
		break;
	case 4: //?????????? ???????????????????? ?????????????????? ?? ?????????????????????????? clear
		printf("!(%s) %s\n", q, f);
		break;
	default: //?????????? ??????????????.
		merror(10, "in function printer default output", 0, 0);
		break;
	}
}

struct fstck find (char *s, char *f) {
	if ( f == NULL) merror(1, " ", 0, 0);
	
	char f_s[MAX_STR_SIZE];
	
	FILE *file;
	file = fopen(f,"rt"); //???????????? ?????? ????????????
	if (file == NULL) merror(3, f, 0, 0);
	
	int done = -1;
	
	struct fstck fstck1;
	
	unsigned int k = 0;

	for (unsigned int i = 0; fgets(f_s, MAX_STR_SIZE, file) != NULL; i++) {

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

struct stack lexer (struct stack stck1) {
	struct stack stck2;
	
	struct cur cr = { 0 , 0 };

	int done = -1;
	unsigned int l = 0;

	char buf[NORM_STR_SIZE];

	for (unsigned int i = 0; i < stck1.size; i++) {

		//printf("%s", stck1.stck[i]);//========================
		
		for (unsigned int j = 0; j < strlen(stck1.stck[i]); j++ ) {
			
			//printf("%c\n", stck1.stck[i][j]);//========================
			switch (stck1.stck[i][j]) {
			case '\n':
			case '\0':
				
				break;
			case '-':
			case '.':
			case '/':
				break;
			case '(':
			case '{':
			case '[':
				break;
			case ')':
			case '}':
			case ']':
				break;
			case ' ':
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
			//case '+':
			case '=':
			case '@':
				break;
			default:
				break;
			}
			
			
			switch (stck1.stck[i][j]) {
			case '\n':
			case '\0':
				cr.j = 0;
				done = 1;
				break;
			case '-':
			case '.':
			case '/':
				done = 1;
				stck2.stck[cr.i][cr.j] = stck1.stck[i][j];
				cr.j++;
				break;
			case '(':
			case '{':
			case '[':
				//printf("%c\n", stck2.stck[cr.i][cr.j]);//========================
				l++;
				done = 1;
				if (cr.j != 0) cr.i++;
				stck2.stck[cr.i][0] = stck1.stck[i][j];;
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
				stck2.stck[cr.i][0] = stck1.stck[i][j];;
				cr.i++;
				cr.j = 0;
				break;
			case ' ':
				if (cr.j != 0) cr.i++;
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
			//case '+':
			case '=':
			case '@':
				done = 1;
				if (cr.j != 0) cr.i++;
				stck2.stck[cr.i][0] = stck1.stck[i][j];;
				cr.i++;
				cr.j = 0;
				break;
			default:
				done = 1;
				stck2.stck[cr.i][cr.j] = stck1.stck[i][j];
				cr.j++;
				break;
			}

			if (done < 0) merror(7, stck1.stck[i], 0, j);
			done = -1;
		}

		stck2.size = cr.i + 1;
		if (l != 0) merror(6, stck1.stck[i], 0, 0);
		return stck2;
	}
}

struct stack parser (struct stack stck1) {
	for (unsigned int i = 0; i < stck1.size; i++) {
		//printf("q = %s t = %s\n", stck1.stck[i]);//=============================
		switch (stck1.stck[i][0]) {
		case '(':
			break;
		case ')':
			break;
		case '+':
			if (stck1.stck[i][1] != '\0') stck1.type[i][0] = 'p';
			else merror(5, stck1.stck[i], 0, 0);
			//printf("fgfgfg\n");
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
			char *home = getenv ("HOME");

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

	char par[NORM_STR_SIZE][5];
	unsigned int par_i = 0;

	for (unsigned int i = 0; i < stck1.size; i++) {
		if (stck1.type[i][0] == 'p') {
			strcpy (par[par_i], stck1.stck[i]);
			par_i++;
			printf("'%s'\n", par[par_i]);
		}
	}

	for (unsigned int i = 0; i < stck1.size; i++) {

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

	runer(parser(lexer(stck1)));
	
}

void runfromfile(char *f) {
	if ( f == NULL) merror(1, " ", 0, 0);
	char f_s[MAX_STR_SIZE];
	FILE *file;
	file = fopen(f,"rt"); //???????????? ?????? ????????????
	if (file == NULL) merror(3, f, 0, 0);

	for (unsigned int i = 0; fgets(f_s, MAX_STR_SIZE, file) != NULL; i++) {
		if ((f_s[0] != ' ' || f_s[1] != ' ') && f_s[0] != '\n') {
			run(f_s);
		}
	}
	fclose(file);
}

	//(tag) /home/krot-dendi2e/my_projects/githuh/chca/README.chca
	//
	
	/*

	return [file | files] [with_i_j] [with comm]

	(tag) /home/krot-dendi2e/my_projects/githuh/chca/README.chca

	*/
	

struct stack input (char *f) {
	if ( f == NULL) merror(1, " ", 0, 0);
	
	char f_s[MAX_STR_SIZE];
	
	FILE *file;
	file = fopen(f,"rt"); //???????????? ?????? ????????????
	if (file == NULL) merror(3, f, 0, 0);
	
	int done = -1;
	
	unsigned int k = 0;

	for (unsigned int i = 0; fgets(f_s, MAX_STR_SIZE, file) != NULL; i++) {

		if (/*f_s[0] != ' ' || f_s[0] != ' '*/1) {

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
			}
		}
	}
	fclose(file);
}

void output(char *s) {
	if (s == NULL) merror(1, " ", 0, 0);
	FILE *file;
	file = fopen(s,"w+");

	if (file == NULL) merror(2, s, 0, 0);
	
	char f_s[MAX_STR_SIZE];
/*
	fprintf(output_file, "<!DOCTYPE html><html");
	if (lang[0] != '\0') fprintf(output_file, " lang=\"%s\"", lang);
	fprintf(output_file, "><head><meta charset=\"utf-8\">");
	if (name[0] != '\0') fprintf(output_file, "<title>%s</title>", name);
	fprintf(output_file, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
	//fprintf(output_file, "<meta name=\"Keywords\" content=\"%s\">", keywords);	
	fprintf(output_file, "</head>\n<body>\n");
	fprintf(output_file, "<h2><b>%s</b></h2>\n", f_s);		
	fprintf(output_file, "<p>");
	fprintf(output_file, "</p>\n");
	fprintf(output_file, "<pre translate=\"no\">%s</pre translate=\"no\">\n", f_s);
	//fprintf(output_file, "%s<br>\n", f_s);
	fprintf(output_file, "<a href='");
	fprintf(output_file, "</a>\n");
	fprintf(output_file, "'>");
	fprintf(output_file, "%c", f_s[j]);
	fprintf(output_file, "<b>%s</b>");
	fprintf(output_file, "<ul>%s</ul>");
	fprintf(output_file, "<li>%s</li>");
	fprintf(output_file, "</body></html>");
	fclose(output_file);
*/
}

void chml(char *inputfile, char *outputfile) {
	//output(input(inputfile), outputfile);
}

int main (int argc, char *argv[]) {
	//???????? ???????????? ???????????????????? ?????? ????????????????
	if (argc > 1) {
		switch (argv[1][0]) {
		case '(':
			run(argv[1]);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			printf("lexem_list:\nh - help;\nv - version;\nr /path/ - run from file;\nother lexem_list and tutorial in file README.chca\n");
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
		case 'w':
			chml(argv[2], argv[3]);
			exit(EXIT_SUCCESS);
			break;
		default:
			printf("try h for help.\n");
			exit(EXIT_SUCCESS);
			break;
		}
		return 0;
	}
	//?????????????????????????? ??????????
	else {
		printf("try h for help.\n");
		for(;;) {
			printf(">");
			char s[NORM_STR_SIZE];
			if (scanf("%s", s) != 1) merror(11, " ", 0, 0);
			switch(s[0]) {
			case '(':
				run(s);
				break;
			case 'r':
				runfromfile(s);
				break;
			case 'h':
				printf("lexem_list:\nh - help;\nv - version;\nr /path/ - run from file;\nq - quit;\nother lexem_list and tutorial in file README.chca;\n");
				break;
			case 'v':
				printf(VERSION);
				break;
			case 'w':
			//	chml(s, s);
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
