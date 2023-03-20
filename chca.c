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
 * author:
 *          Jerzy Pavka
 *          mail jerzypavka@gmail.com
 *          site https://irrumator228.github.io/
 */

#define VERSION "chca-0.0.3-egg (c) 2023 Jerzy Pavka\n"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define PRINT_BUF     60
#define MIN_PRINT_BUF 20
#define MIN_STR       10
#define NORM_STR      100
#define MAX_STR       1000
#define MAX_PATH      100

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//errors
#define E_PATH      1  //path not specified
#define E_COM       2  //command not found
#define E_FOPEN     3  //open file
#define E_FCLOSE    4  //close file
#define E_UNK_OP    5  //unknown operator
#define E_SEP       6  //extra closing/opening parenthesis/separator
#define E_UNK_C     7  //unknown character
#define E_BUF       8  //buffer overflow
#define E_NODIR     9  //no such file or directory
#define E_SYS       10 //error in code

char *get_homedir(void) {
    char homedir[MAX_PATH];
#ifdef _WIN32
    snprintf(homedir, MAX_PATH, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
#else
    snprintf(homedir, MAX_PATH, "%s", getenv("HOME"));
#endif
    return strdup(homedir);
}

typedef struct string {
	char s[MAX_STR];
} string;

struct stack {
	char stck[NORM_STR][NORM_STR];
	unsigned int len[NORM_STR];
	unsigned int size;
	char type[NORM_STR][5];

	//char f[NORM_STR][NORM_STR];
	//char s[NORM_STR][MAX_STR];
	//unsigned int i[MAX_STR];
	//unsigned int j[MAX_STR];
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
	char query[NORM_STR][NORM_STR];
	char f[NORM_STR][NORM_STR];
	char s[NORM_STR][MAX_STR];
	unsigned int i[MAX_STR];
	unsigned int j[MAX_STR];
	unsigned int je[MAX_STR];
	unsigned int size;
};

typedef struct file_entry_list {
	//int status; //1 - entry not found, 0 - entry exists
	char file[NORM_STR];
	char entry[PRINT_BUF];
	unsigned int i;
	unsigned int j;
} file_entry_list;

typedef struct file_entry_list_head {
	struct file_entry_list *head;
} file_entry_list_head;

typedef struct lexem_list {
	char lexem[NORM_STR];
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
}
*/

void push_st(lexem_list_head *l) {
	lexem_list *tmp = (lexem_list*) malloc(sizeof(lexem_list));
	//tmp->

}

void new_st(lexem_list_head *l, lexem_list *target) {
	
}

/*int*/ char *printi (/*char result[MAX_STR],*/ char *s, unsigned int j) {
	char result_1[MIN_PRINT_BUF];
	char result_2[PRINT_BUF - MIN_PRINT_BUF];
	//char *result = malloc(sizeof(char) * MAX_STR);
	char result[MAX_STR];
	char *result_p = result;
	unsigned int result_i = 0;
	if (s[strlen(s) - 1] == '\n') s[strlen(s) - 1] = ' ';
	for (unsigned int i = j >= MIN_PRINT_BUF ? j - MIN_PRINT_BUF : 0; i < j - 1; i++) {
		result_1[result_i] = s[i];
		result_i++;
	}
	result_1[result_i] = '\0';
	result_i = 0;
	for (unsigned int i = j - 1; i < strlen(s) && i < j + PRINT_BUF-MIN_PRINT_BUF; i++) {
		result_2[result_i] = s[i];
		result_i++;
	}
	result_2[result_i] = '\0';

	snprintf(result, MAX_STR, "  %s %s->%s %s", result_1, KMAG, KNRM, result_2); 
	//return 0;
	return result_p;
}

void merror (unsigned int n, char *s, unsigned int  i, unsigned int j) {
	char *err_s;
	switch (n) {
	case E_PATH:
		err_s = "path not specified";
		break;
	case E_COM:
		err_s = "command not found";
		break;
	case E_FOPEN:
		err_s = "open file";
		break;
	case E_FCLOSE:
		err_s = "close file";
		break;
	case E_UNK_OP:
		err_s = "unknown operator";
		break;
	case E_SEP:
		err_s = "extra closing/opening parenthesis";
		break;
	case E_UNK_C:
		err_s = "unknown character";
		break;
	case E_BUF:
		err_s = "buffer overflow";
		break;
	case E_NODIR:
		err_s = "it's no such file or directory";
		break;
	case E_SYS:
		err_s = " ";
		break;
	default:
		err_s = "unknown error";
		break;
	}
	//вывод без файла, строки и номера символа 
	if ((i == 0) && (j == 0)) {
		printf("%s\n%u error. %s.\n", s, n, err_s);
	}
	//вывод строки и номера символа
	else if (i == 0) {
		//char buf[MAX_STR];
		//printi(buf, s, j);
		printf("%s\n%u error. %s:%u\n", /*buf,*/ printi(s,j), n, err_s, j);
	}
	//вывод файла, строки и номера символа
	else {
		//char buf[MAX_STR];
		//printi(buf,s,j);
		printf("%s\n%u error. %s. %s:%u:%u\n", /*buf,*/ printi(s,j), n, err_s, s, i, j);
	}
	exit(EXIT_FAILURE);
}

void printer (unsigned int code, char *s, char *f, char *q, unsigned int i, unsigned int j) {
	switch (code) {
	case 1: //вывод вхождения из файла
		char buf[MAX_STR];
		//printi(buf,s,j);
		printf("%s\n", /*buf*/ printi(s,j));
		printf("(%s) %s:%u:%u\n", q, f, i, j);
		break;
	case 2: //вывод отсутствия вхождения
		printf("  no entries found\n");
		printf("!(%s) %s\n", q, f);
		break;
	case 3: //вывод вхождения из файла с модификатором clear
		printf("(%s) %s:%u:%u\n", q, f, i, j);
		break;
	case 4: //вывод отсутствия вхождения с модификатором clear
		printf("!(%s) %s\n", q, f);
		break;
	default: //вывод дефолта.
		merror(E_SYS , "in function printer default output", 0, 0);
		break;
	}
}

struct fstck find (char *s, char *f) {
	if ( f == NULL) merror(E_PATH, " ", 0, 0);
	
	char f_s[MAX_STR];
	
	FILE *file;
	file = fopen(f,"rt"); //только для чтения
	if (file == NULL) merror(E_FOPEN , f, 0, 0);
	
	int done = -1;
	
	struct fstck fstck1;
	
	unsigned int k = 0;

	for (unsigned int i = 0; fgets(f_s, MAX_STR, file) != NULL; i++) {

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
	
	if (!d) merror(E_PATH, s, 0, 0);
	
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

	char buf[NORM_STR];

	for (unsigned int i = 0; i < stck1.size; i++) {

		//lexem_list_head 

		//printf("%s", stck1.stck[i]);//========================
		
		for (unsigned int j = 0; j < strlen(stck1.stck[i]); j++ ) {
			
			//printf("%c\n", stck1.stck[i][j]);//========================
			switch (stck1.stck[i][j]) {
			case '\n':
			case '\0':
			case '-':
			case '.':
			case '/':
			case '(':
			case '{':
			case '[':
			case ')':
			case '}':
			case ']':
			case ' ':
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

			if (done < 0) merror(E_UNK_C, stck1.stck[i], 0, j);
			done = -1;
		}

		stck2.size = cr.i + 1;
		if (l != 0) merror(E_SEP, stck1.stck[i], 0, 0);
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
			else merror(E_UNK_OP , stck1.stck[i], 0, 0);
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
			//char path[MAX_PATH];
			//snprintf(path, MAX_PATH, "%s%s", get_homedir(), );

		case '/':
			if (opendir(stck1.stck[i]) != NULL) { stck1.type[i][0] = 'd'; }
			else if (fopen(stck1.stck[i],"rt") != NULL) { stck1.type[i][0] = 'f'; }
			else merror(E_NODIR, stck1.stck[i], 0, 0);
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

	char par[NORM_STR][5];
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
		merror(E_UNK_OP ," ",0,0);
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
	if ( f == NULL) merror(E_PATH, " ", 0, 0);
	char f_s[MAX_STR];
	FILE *file;
	file = fopen(f,"rt"); //только для чтения
	if (file == NULL) merror(E_FOPEN , f, 0, 0);

	for (unsigned int i = 0; fgets(f_s, MAX_STR, file) != NULL; i++) {
		if ((f_s[0] != ' ' || f_s[1] != ' ') && f_s[0] != '\n') {
			run(f_s);
		}
	}
	fclose(file);
}

	//(tag) /home/krot-dendi2e/my_projects/githuh/chca/README.chca
	/*
	return [file | files] [with_i_j] [with comm]
	(tag) /home/krot-dendi2e/my_projects/githuh/chca/README.chca
	*/

struct stack input (char *f) {
	if ( f == NULL) merror(E_PATH, " ", 0, 0);
	
	char f_s[MAX_STR];
	
	FILE *file;
	file = fopen(f,"rt"); //только для чтения
	if (file == NULL) merror(E_FOPEN , f, 0, 0);
	
	int done = -1;
	
	unsigned int k = 0;

	for (unsigned int i = 0; fgets(f_s, MAX_STR, file) != NULL; i++) {

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
	if (s == NULL) merror(E_PATH, " ", 0, 0);
	FILE *file;
	file = fopen(s,"w+");

	if (file == NULL) merror(E_COM, s, 0, 0);
	
	char f_s[MAX_STR];
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
	switch (argv[1][0]) {
	case 'h':
		printf("commands:\nh - help;\nv - version;\nr /path/ - run from file;\nother lexem_list and tutorial in file README.chca\n");
		break;
	case 'v':
		printf("%s\n", VERSION);
		break;
	case 'r':
		runfromfile(argv[2]);
		break;
	case 'w':
		chml(argv[2], argv[3]);
		break;
	default:
		printf("try h for help.\n");
		break;
	}
	exit(EXIT_SUCCESS);
}
