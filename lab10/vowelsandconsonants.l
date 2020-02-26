%{
	#include<stdio.h>
	int linecount = 0;
	int vowelcount = 0; 
	int consonantcount = 0; 
%}

%%
[aeiou] {vowelcount++;}
[a-zA-Z]  consonantcount++;
\n {linecount++;}
%%
int main()
{ 
	yylex();
	printf("no of lines are %d\n",linecount);
	printf("no of vowels are %d\n",vowelcount);
	printf("no of consonants are %d\n",consonantcount);
	return 0;
}
int yywrap()
{
	return 1;
} 