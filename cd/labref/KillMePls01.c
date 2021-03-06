

//LEXICAL ANALYZER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token{
	char ln[100];
	int row, col, type;
};
typedef struct token* tokenptr;

int isKeyWord(char *str){
	char keyw[][32]={"auto","break","case","char",
	"const","continue","default","do",
	"double","else","enum","extern",
	"float","for","goto","if",
	"int","long","register","return",
	"short","signed","sizeof","static",
	"struct","switch","typedef","union",
	"unsigned","void","volatile","while"};
	
	//printf("\nNow comparing **%s**\n",str);
	
	for(int i=0;i<31;i++){
		if(strcmp(str,keyw[i])==0)
			return 1;
	}
	return 0;
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
	
	//UPDATE ROW AND COLUMN ON NEWLINE
	if(ch == '\n'){
		*c=0;
		(*l)++;
		nt->type = -1;
		return nt;
	}

	//IGNORING ALL WHITE SPACES
	if(ch == ' ' || ch =='\t'){
		(*c)++;
		nt->type = -1;
		return nt;
	}

	//IGNORING PREPROCESSOR DIRECTIVES
	if(ch == '#'){
		ch = getc(fa);
		while(ch!='\n')
			ch = getc(fa);
		(*l)++;
		nt->type = -1;
		return nt;
	}

	//IGNORING COMMENTS
	temp=ch;
	if(temp=='/'){
		temp = getc(fa);
		if(temp == '*'){    //MULTILINE COMMENTS
			*c+=2;
			while(1){
				temp = getc(fa);
				(*c)++;
				if(temp == '\n'){
					*c = 0;
					(*l)++;
				}
				if(temp == '*'){
					char temp2=getc(fa);
					if(temp2 == '/'){
						(*c)++;
						nt->type = 0;
						return nt;
					}
					fseek(fa, -1, SEEK_CUR);
				}
			}
		}
		else if(temp == '/'){   //SINGLE LINE COMMENTS
			*c+=2;
			while(1){
				temp = getc(fa);
				(*c)++;
				if(temp == '\n'){
					*c = 0;
					(*l)++;
					nt->type = 0;
					return nt;
				}
			}	
		}
	}

	//STRING LITERALS
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

	//ARITHMETIC OPERATORS
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

	//RELATIONAL OPERATOR
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
			if(a[0]=='!')                //LOGICAL OPERATOR !
				nt->type = 2;
			else if(a[0]=='=')           //ASSIGNMENT OPERATOR ==
				nt->type = 11;
			else						 //RELATIONAL OPERATOR
				nt->type = 1;
			strcpy(nt->ln, a);
			fseek(fa, -1, SEEK_CUR);
			return nt;
		}
	}

	//Alpha Stuff --> variable identifier or a keyword
	if(isalpha(ch)||ch=='_'){
		(*c)++;
		ch = getc(fa);
		while(isalpha(ch)||ch=='_'||(ch>='0'&&ch<='9')){
			a[i++]=ch;
			(*c)++;
			ch = getc(fa);
		}
		a[i]='\0';
		fseek(fa, -1, SEEK_CUR);
		strcpy(nt->ln, a);
		if(isKeyWord(a)==1){  //Keyword
				nt->type=4;
				return nt;
			}
		nt->type = 7;  //IDENTIFIER
		return nt;
	}

	//IF IT STARTS WITH A NUM
	//Num literal
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

	//Logical Operator
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

	a[i]='\0';
	strcpy(nt->ln, a);
	nt->type = 3;
	return nt;
}

int main(){
	FILE *fa;
	char types[][20]={"ArithmeticOperator", "RelationalOperator", "LogicalOperator", "SpecialSymbol", "Keyword", "NumConstant", "StrLiteral", "Identifier", "ShorthandOperator","IncrementOperator","DecrementOperator","AssignmentOperator"};
	
	int l=1, c=0;
	char inname[100];
	printf("Enter input filename: ");
	scanf(" %s", inname);

	fa = fopen(inname, "r");

	if (fa == NULL){
		printf("Cannot open file \n");
		exit(0); 
	}
	printf("\nTokens:\n\n");
	tokenptr temp;
	do{
		temp = getNextToken(fa, &l, &c);
		if(temp && temp->type!=-1)
			printf("<%-30s,%-5d,%-5d,%-30s >\n", temp->ln, temp->row, temp->col, types[temp->type]);
	}while(temp!=NULL);
}
