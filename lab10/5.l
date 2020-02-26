%{
#include <stdio.h>
int f=0;
%}

%%
("and"|"or"|"if") {f=1;}
%%

void main(int argc, char*argv[])
{
	yylex();

	if(f==1)
		printf("Compound");
	else
		printf("Not Compound");


}
int yywrap(){
		return 1;
	}