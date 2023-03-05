#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>
#include <uchar.h> 

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

int runer(char *inputf_s, char *outputf_s) {
	if ( inputf_s == NULL) merror(1, " ", 0, 0);
	if ( outputf_s == NULL) merror(1, " ", 0, 0);
	FILE *input_file;
	FILE *output_file;

	input_file = fopen(inputf_s,"rt");
	output_file = fopen(outputf_s,"w+");

	if (input_file == NULL) merror(2, inputf_s, 0, 0);
	if (output_file == NULL) merror(2, outputf_s, 0, 0);

	fprintf(output_file, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\">");

	//fprintf(output_file, "<!DOCTYPE html><html lang=\"%s\"><head><meta charset=\"utf-8\">", lang);

	//fprintf(output_file, "<title>%s</title>", name);

	//fprintf(output_file, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");

	//fprintf(output_file, "<meta name=\"Keywords\" content=\"%s\">", keywords);
	
	fprintf(output_file, "</head><body>");
	
	char f_s[1000];
	unsigned int l = 0;
	for (unsigned int i = 0; fgets(f_s, 1000, input_file) != NULL; i++) {
		for (unsigned int j = 0; j < strlen(f_s); j++) {
			if (f_s[j] == '\n') f_s[j] = '\0';
		}
		switch(f_s[0]) {
		case '/':
			fprintf(output_file, "<h2><b>%s</b></h2>", f_s);
			break;
		case '#':
			break;
		case '{':
			l++;
			fprintf(output_file, "<p>");
			break;
		case '}':
			l--;
			fprintf(output_file, "</p>");
			break;
		case '\n':
		case '\0':
			break;
		case ' ':
			if (f_s[1] == ' ') break;
			//if not ' ', run default
		default:
			fprintf(output_file, "%s<br>", f_s);
			break;
		}
		for (unsigned int j = 0; j < strlen(f_s); j++) {
			switch(f_s[j]) {
			case '[':
				
				break;
			default:
				break;
			}
		}
	}
	if (l != 0) merror(4, " ", 0, 0);
	fprintf(output_file, "</body></html>");
	fclose(input_file);
	fclose(output_file);
}

int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE, "Russian.UTF-8"); /*change your language*/

	if (argv[1] != NULL && argv[2] != NULL) runer(argv[1], argv[2]);
	else printf("write as the arguments paths for input and output files");
	return 0;
}
	/*
	fprintf(file, "<body>%s</body></html>");

	fprintf(file, "<pre translate=\"no\">%s</pre translate=\"no\">");

	fprintf(file, "<a href='%s'>%s</a>");

	fprintf(file, "<h2><b>%s</b></h2>");

	fprintf(file, "<b>%s</b>");

	fprintf(file, "%s</p>");

	fprintf(file, "<ul>%s</ul>");

	fprintf(file, "<li>%s</li>");
	*/