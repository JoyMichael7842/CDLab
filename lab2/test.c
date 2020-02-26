#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token{
	char ln[100];
	int row,col,type;
};
typedef struct Token* tokenptr;

tokenptr getNextToken(FILE *fa,int *l,int *c){
	char a[100],ch,temp,i=0,keys[][20]={"int", "char", "if", "else", "while", "void", "for", "return"}
	tokenptr newToken=(tokenptr)malloc(sizeof(struct Token));

	ch = getc(fa);
	if(ch==EOF){
		newToken = NULL;
		return newToken;
	}
	a[i]=ch;
	i++;
	newToken->row = *l;
	newToken->col = *c;

	if(ch=='\n'){
		*c=0;
		(*l)++;
		newToken->type = 0;
		return newToken;
	}

	if(ch=='#'){
		ch = getc(fa);
		while(ch!='\n')
			ch = getc(fa);
		(*l)++;
		newToken->type = 0;
		return newToken;
	}
	


}


int main(){
	FILE *fa,*fb;
	char types[][20] = {"No_Token", "Arithmetic_Operator", "Relational_Operator", "Logical_Operator", "Special_Symbol", "Keyword", "Number_Constant", "String_Literal", "Identifier", "Shorthand_Operator", "Assignment_Operator"};
	int l=1,c=0;

	char inname[100];
	printf("Enter input filename: ");
	scanf(" %s", inname);

	fa = fopen(inname, "r");
	fb = fopen("result.txt","w");

	if (fa == NULL){
		printf("Cannot open file \n");
		exit(0); 
	}

	tokenptr temp;
	do{
		temp=getNextToken(fa,&l,&c);
		if(temp && temp->type!=0)
			fprintf(fb,"<  '%s' , %d , %d , %s  >\n", temp->ln, temp->row, temp->col, types[temp->type]);
	}while(temp!=NULL);
}