%{
	#include<stdio.h>
	int wordcount = 0;
	int charectercount = 0; 
	int blankcount = 0;
	int linecount = 0; 
%}

%%
\n {linecount++;}
[" "] {}
[aeiou] {vowelcount++;}
[a-zA-Z]  consonantcount++;

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