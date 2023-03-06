#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char stack[100][100];

void merror (unsigned int n, char *s, unsigned int i, unsigned int j) {
	char *err_s;
	switch (n) {
	case 1:
		err_s = "path not specified";
		break;
	case 2:
		err_s = "open file";
		break;
	case 3:
		err_s = "close file";
		break;
	case 4:
		err_s = "extra closing/opening parenthesis";
		break;
	default:
		err_s = "unknow error";			
	}
	printf("%u error. %s - %s\n", n, err_s, s);
	exit(EXIT_FAILURE);
}

void input (char *f) {
	if ( f == NULL) merror(1, " ", 0, 0);
	
	char f_s[1000];
	
	FILE *file;
	file = fopen(f,"rt"); //только для чтения
	if (file == NULL) merror(3, f, 0, 0);
	
	int done = -1;
	
	unsigned int k = 0;

	for (unsigned int i = 0; fgets(f_s, 1000, file) != NULL; i++) {

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
	
}

int runer(char *inputf_s, char *outputf_s) {
	if ( inputf_s == NULL) merror(1, " ", 0, 0);
	if ( outputf_s == NULL) merror(1, " ", 0, 0);
	FILE *input_file;
	FILE *output_file;

	input_file = fopen(inputf_s,"rt");
	output_file = fopen(outputf_s,"w+");

	if (input_file == NULL) merror(2, inputf_s, 0, 0);
	if (output_file == NULL) merror(2, outputf_s, 0, 0);

	

	char lang[5]; //ограничение
	char name[100]; //ограничение
	
	char f_s[1000];

	for (unsigned int i = 0; fgets(f_s, 1000, input_file) != NULL; i++) {
		if (f_s[0] == '#') {
			if (f_s[1] == 'l' && 
			    f_s[2] == 'a' && 
				f_s[3] == 'n' && 
				f_s[4] == 'g')
					for (uint j = 0; f_s[j+6] != '\0' && j < 5 /* <- вот оно*/ ; j++) lang[j] = f_s[j + 6]; //ограничение
			if (f_s[1] == 'n' && 
			    f_s[2] == 'a' && 
				f_s[3] == 'm' && 
				f_s[4] == 'e')
					for (uint j = 0; f_s[j+6] != '\0' && j < 100 /* <- вот оно*/ ; j++) name[j] = f_s[j + 6]; //ограничение
		}
	}
	fclose(input_file);
	input_file = fopen(inputf_s,"rt");


	fprintf(output_file, "<!DOCTYPE html><html");
	if (lang[0] != '\0') fprintf(output_file, " lang=\"%s\"", lang);
	fprintf(output_file, "><head><meta charset=\"utf-8\">");
	if (name[0] != '\0') fprintf(output_file, "<title>%s</title>", name);
	fprintf(output_file, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");

	//fprintf(output_file, "<meta name=\"Keywords\" content=\"%s\">", keywords);
	
	fprintf(output_file, "</head>\n<body>\n");

	unsigned int l = 0;
	for (unsigned int i = 0; fgets(f_s, 1000, input_file) != NULL; i++) {
		for (unsigned int j = 0; j < strlen(f_s); j++) {
			if (f_s[j] == '\n') f_s[j] = '\0';
		}
		switch(f_s[0]) {
		case '/':
			f_s[0] = ' ';
			fprintf(output_file, "<h2><b>%s</b></h2>\n", f_s);
			break;
		case '#':
			break;
		case '{':
			l++;
			fprintf(output_file, "<p>");
			break;
		case '}':
			l--;
			fprintf(output_file, "</p>\n");
			break;
		case '\n':
		case '\0':
			break;
		case ' ':
			if (f_s[3] == '=') break;
 			if (f_s[1] == ' ') {
				fprintf(output_file, "<pre translate=\"no\">%s</pre translate=\"no\">\n", f_s);
				break;
			}
			//if not ' ', run default
		default:
			//fprintf(output_file, "%s<br>\n", f_s);
			for (unsigned int j = 0; j < strlen(f_s); j++) {
				switch(f_s[j]) {
				case '[':
					fprintf(output_file, "<a href='");
					break;
				case ']':
					fprintf(output_file, "</a>\n");
					break;
				case '|':
					fprintf(output_file, "'>");
					break;
				default:
					fprintf(output_file, "%c", f_s[j]);
					break;
				}
			}
			break;
		}
	}
	if (l != 0) merror(4, " ", 0, 0);
	fprintf(output_file, "</body></html>");
	fclose(input_file);
	fclose(output_file);
}

int main(int argc, char *argv[]) {

	if (argv[1] != NULL && argv[2] != NULL) runer(argv[1], argv[2]);
	else printf("write as the arguments paths for input and output files");
	return 0;
}
	/*



	






	<b>%s</b>

	<ul>%s</ul>

	<li>%s</li>
	*/