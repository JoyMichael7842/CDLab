
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct token{
	char ln[100];
	int row, col, type;
};
typedef struct token* tokenptr;

bool isKeyWord(char *str){
	char keyw[][32]={"do",
	"float","for","goto","if",
	"int","long","register","return",
	"short","signed","sizeof","static",
	"struct","switch","typedef","union",
	"unsigned","void","volatile","while","foreach","scalar"};
	
	
	
	for(int i=0;i<31;i++){
		if(strcmp(str,keyw[i])==0)
			return true;
	}
	return false;
}

tokenptr getNextToken(FILE *fa, int *l, int *c){
	char a[100], ch, temp;
	int i=0;
	tokenptr nt=(tokenptr)malloc(sizeof(struct token));
	ch = getc(fa);
	if(ch==EOF){
		nt = NULL;
		return nt;
	}
	a[i++]=ch;
	nt->row = *l;
	nt->col = *c;
	
	
	if(ch == '\n'){
		*c=0;
		(*l)++;
		nt->type = -1;
		return nt;
	}
	
	
	if(ch == ' ' || ch =='\t'){
		(*c)++;
		nt->type = -1;
		return nt;
	}

	if(ch=='#'){
		ch=getc(fa);
		if(ch=='!'){
			*c+=2;
			char str[100];int j=0;

			while(ch!='\n'){ 
				ch=getc(fa);
				str[j++]=ch;
			}
			str[--j]='\0';
			printf("Path is %s\n",str);
			(*l)++;(*c)=0;
			strcpy(nt->ln,str);
			nt->type=14;
			return nt;
		}
		else{

			while(ch!='\n') ch=getc(fa);
			(*l)++;(*c)=0;
			nt->type=-1;
			return nt;
		}
	}


	if(ch == '"'){
		(*c)++;
		ch = getc(fa);
		while(ch!='"'){
			(*c)++;
			a[i++]=ch;
			ch = getc(fa);
		}
		a[i]=ch;
		a[i+1]='\0';
		strcpy(nt->ln, a);
		nt->type = 6;
		return nt;
	}


	if(ch=='+'||ch=='-'||ch=='/'||ch=='*'||ch=='%'){
		(*c)++;
		char temp2;
		temp2=getc(fa);
		if(temp2=='='){
			a[i++] = temp2;
			(*c)++;
			a[i] = '\0';
			nt->type = 8;
			strcpy(nt->ln, a);
			return nt;
		}
		else if(ch=='+'&&temp2=='+'){
			a[i++]=temp2;
			(*c)++;
			a[i]='\0';
			nt->type=9;
			strcpy(nt->ln,a);
		}
		else if(ch=='-'&&temp2=='-'){
			a[i++]=temp2;
			(*c)++;
			a[i]='\0';
			nt->type=10;
			strcpy(nt->ln,a);
		}
		else{
			a[i]='\0';
			nt->type = 0;
			strcpy(nt->ln, a);
			fseek(fa, -1, SEEK_CUR);
			return nt;
		}
	}


	if(ch == '<' || ch == '>' || ch == '!' || ch == '='){
		(*c)++;
		ch = getc(fa);
		if(ch=='='){
			a[i++] = ch;
			(*c)++;
			a[i] = '\0';
			nt->type = 1;
			strcpy(nt->ln, a);
			return nt;
		}
		else{
			a[i]='\0';
			if(a[0]=='!')                
				nt->type = 2;
			else if(a[0]=='=')           
				nt->type = 11;
			else						 
				nt->type = 1;
			strcpy(nt->ln, a);
			fseek(fa, -1, SEEK_CUR);
			return nt;
		}
	}

	if(ch=='$'){
		(*c)++;
		ch=getc(fa);
		if(isalpha(ch)||ch=='_'){
			(*c)++;
			a[i++]=ch;
			ch = getc(fa);
			while(isalpha(ch)||ch=='_'||(ch>='0'&&ch<='9')){
				a[i++]=ch;
				(*c)++;
				ch = getc(fa);
			}
			a[i]='\0';
			fseek(fa, -1, SEEK_CUR);
			strcpy(nt->ln, a);
			nt->type = 7;  //IDENTIFIER
			return nt;
		}
		a[i]='\0';
		strcpy(nt->ln,a);
		fseek(fa,-1,SEEK_CUR);
		nt->type=3;
		return nt;
	}

	if(isalpha(ch)){
		(*c)++;
		ch=getc(fa);
		while(isalpha(ch)){
			a[i++]=ch;
			(*c)++;
			ch=getc(fa);
		}
		a[i]='\0';
		strcpy(nt->ln,a);
		fseek(fa,-1,SEEK_CUR);
		if(isKeyWord(a)==true){  //Keyword
				nt->type=4;
				return nt;
			}
		nt->type = -1;
		return nt;
	}

	
	if(isdigit(ch)){
		(*c)++;
		ch = getc(fa);
		while(isdigit(ch)||ch=='.'){
			a[i++]=ch;
			(*c)++;
			ch = getc(fa);
		}
		a[i]='\0';
		fseek(fa, -1, SEEK_CUR);
		strcpy(nt->ln, a);
		nt->type = 5;
		return nt;
	}

	if(ch=='&'||ch=='|'){
		(*c)++;
		char ch2=getc(fa);
		if((ch=='&'&&ch2=='&')||(ch=='|'&&ch2=='|')){ a[i++]=ch2;(*c)++;}	
		else fseek(fa,-1,SEEK_CUR);
		a[i]='\0';
		strcpy(nt->ln,a);
		nt->type=2;
		return nt;
	}

	if(ch=='@'){
		(*c)++;
		ch=getc(fa);
		if(ch=='_'){
			a[i++]=ch;
			a[i]='\0';
			strcpy(nt->ln,a);
			nt->type=13;
			return nt;
		}
		fseek(fa,-1,SEEK_CUR);
	}

	a[i]='\0';
	(*c)++;
	strcpy(nt->ln, a);
	nt->type = 3;
	return nt;
}

int main(){
	FILE *fa;
	char types[][20]={"ArithmeticOperator", "RelationalOperator", "LogicalOperator", "SpecialSymbol",
	 "Keyword", "NumConstant", "StrLiteral", "Identifier", "ShorthandOperator","IncrementOperator","AssignmentOperator","Datatype","SingleToken","PATH"};
	
	int l=1, c=0;
	char inname[100];
	printf("Enter input filename: ");
	scanf(" %s", inname);

	fa = fopen(inname, "r");

	if (fa == NULL){
		printf("Cannot open file \n");
		exit(0); 
	}
	int j=0;
	printf("\nTokens:\n\n");
	tokenptr temp;
	do{
		temp = getNextToken(fa, &l, &c);
		if(temp && temp->type==4)
			printf("<%s>", temp->ln);
		else if(temp && temp->type==7)
			printf("<id,%d>\n",j++);
		else if(temp&& temp->type!=-1)
			printf("<%s>",temp->ln);
	}while(temp!=NULL);
}
