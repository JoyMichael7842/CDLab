%{
#include <stdio.h>
int pf = 0;
int sf = 0;
int others = 0;
%}

%%
"printf" {pf++;fprintf(yyout,"READ");}
"scanf" {sf++;fprintf(yyout,"WRITE");}
^["printf""scanf"] {ECHO;}
%%

void main(int argc, char*argv[])
{

	yyin=fopen("a.txt","r");
	yyout=fopen("b.txt","w");

	yylex();
	printf("Printf - %d\nScanf - %d\n",pf,sf);
}
int yywrap(){
		return 1;
	}