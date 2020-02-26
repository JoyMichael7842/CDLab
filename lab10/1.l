%{
#include <stdio.h>
int vowels = 0;
int consonents = 0;
int others = 0;
%}

%%
[^A-Z,^a-z] {others++;}
[a,e,i,o,u,A,E,I,O,U] {vowels++;}
[^a,^e,^i,^o,^u,^A,^E,^I,^O,^U] {consonents++;}


%%

void main(int argc, char*argv[])
{

	yylex();
	printf("Vowels - %d\nConsonents - %d\n",vowels,consonents);
}
int yywrap(){
		return 1;
	}