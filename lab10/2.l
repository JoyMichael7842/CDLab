%{
#include <stdio.h>
int chars = 0;
int blanks = 0;
int lines =0;
int words = 0;
int others = 0;
%}

%%
[A-Za-z] {chars++;}
" " {chars++; words++;blanks++;}
\n {chars++; lines++;words++;}
[^A-Z,^a-z] {others++;}
%%

void main(int argc, char*argv[])
{

	yylex();
	printf("Words - %d\nLines - %d\nChars - %d\nBlanks - %d\n",words,lines,chars,blanks);
}
int yywrap(){
		return 1;
	}