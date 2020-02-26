%{
#include <stdio.h>
int pin = 0;
int ni = 0;
int pfi =0;
int nfi = 0;
int others = 0;
%}

%%
[-][0-9]+[\.][0-9]+ {nfi++;}
[+][0-9]+[\.][0-9]+ {pfi++;}
[-][0-9]+ {ni++;}
[+][0-9]+ {pin++;}

%%

void main(int argc, char*argv[])
{

	yylex();
	printf("Positive Int - %d\nNegative Int - %d\nPositive Float - %d\nNegative Float - %d\n",pin,ni,pfi,nfi);
}
int yywrap(){
		return 1;
	}