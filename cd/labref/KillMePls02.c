
//SYMBOL TABLE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//#include "Helper1.h"
#define TLEN 30


/*
Token Types

-1:Invalid
 0:Arithmetic Operator
 1:Relational Operator
 2:Logical Operator
 3:Special Symbol
 4:Keyword
 5:Numerical Constant
 6:String Literal
 7:Identifier
 8:ShortHand Operator
 9:Increment Operator
10:Decrement Operator
11:Assignment Operator
12:Datatype
*/

enum tokenType{EOFILE=-1,LT,LTE,GT,GTE,E,NE};
typedef struct token{
	char ln[100];
	int row, col, type;
	int index;
}token;
typedef struct token* tokenptr;

bool isDatatype(const char* token)
{
    const char *datatype[]={"char","double","float","int","long"};
    for(size_t i=0;i<(sizeof(datatype)/sizeof(char*));i++)
    {
        if(strcmp(token,datatype[i])==0)
            return true;
    }
    return false;
}

bool isKeyWord(char *str){
	char keyw[][32]={"auto","break","case","char",
	"const","continue","default","do",
	"double","else","enum","extern",
	"float","for","goto","if",
	"int","long","register","return",
	"short","signed","sizeof","static",
	"struct","switch","typedef","union",
	"unsigned","void","volatile","while"};
	
	
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
		if(isDatatype(a)){          //if a datatype
			nt->type=12;
			return nt;
		}
		if(isKeyWord(a)){  //Keyword
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
		if((ch=='&'&&ch2=='&')||(ch=='|'&&ch2=='|')){ a[i++]=ch2,(*c)++;}
		else fseek(fa,-1,SEEK_CUR);
		a[i]='\0';
		strcpy(nt->ln,a);
		nt->type=2;
		return nt;
	}

	a[i]='\0';
	(*c)++;
	strcpy(nt->ln, a);
	nt->type = 3;
	return nt;
}


int countB=0;
int countC=0;

typedef struct ListElement{
	char name[100];
	char type[100];
	int size;
	char scope;
	int nargs;
	char rtype[100];
	struct ListElement* next;
}ListElement;

typedef ListElement* Listptr;

Listptr st[TLEN];

void initialize(){
	for (int i = 0; i < TLEN; ++i)
		st[i] = NULL;
}

int hash(char *str){
	int len = strlen(str);
	int val = 0;
	for (int i = 0; i < len; i++)
		val += (i+1) * str[i];
	return val % TLEN;
}

bool search(char* str){
	int h=hash(str);
	if(st[h]==NULL) return false;
	Listptr curr=st[h];
	while(curr!=NULL){
		if(strcmp(curr->name,str)==0) return true;
		curr=curr->next;
	}
	return false;
}

void insert(char* n,char* t,int s,char scop,int no,char* rt){
	if(search(n)==false){
		ListElement* temp=(ListElement*)malloc(sizeof(ListElement));
		strcpy(temp->name,n);
		strcpy(temp->type,t);
		temp->size=s;
		temp->scope=scop;
		temp->nargs=no;
		strcpy(temp->rtype,rt);
		temp->next=NULL;

		int h=hash(n);
		if(st[h]==NULL){
			st[h]=temp;
			return;
		}
		Listptr curr=st[h];
		while(curr->next!=NULL){
			curr=curr->next;	
		}
		curr->next=temp;
		return;
	}
}

void display(){
	printf("\n\nSYMBOL TABLE\n\n");
	printf("%-10s  %-10s  %-4s  %-5s  %-17s  ReturnType\n","Name","Type","Size","Scope","NumberOfArguments");
	for(int i=0;i<TLEN;i++){
		Listptr curr=st[i];
		if(curr==NULL) continue;
		while(curr!=NULL){
			//printf("");
			if(strcmp(curr->type,"FUNC")==0)
				printf("%-10s  %-10s  %-4s  %-5s  %-17d  %s\n",curr->name,curr->type,"----","----",curr->nargs,curr->rtype);
			else
				printf("%-10s  %-10s  %-4d  %-5c  %-17s  %s\n",curr->name,curr->type,curr->size,curr->scope,"----",curr->rtype);
			curr=curr->next;
		}
	}
}


int getSize(char str[]){
	if(strcmp(str,"int")==0) return 4;
	if(strcmp(str,"float")==0) return 4;
	if(strcmp(str,"double")==0) return 8;
	if(strcmp(str,"char")==0) return 2;
	return -1;
}

int main(){
	FILE *fa;
	int l=1, c=0;
	char inname[100];
	printf("Enter input filename: ");
	scanf(" %s", inname);
	char types[][20]={"ArithmeticOperator", "RelationalOperator", "LogicalOperator", "SpecialSymbol", "Keyword", "NumConstant", "StrLiteral", "Identifier", "ShorthandOperator","IncrementOperator","DecrementOperator","AssignmentOperator","DATATYPE"};

	fa = fopen(inname, "r");

	if (fa == NULL){
		printf("Cannot open file \n");
		exit(0); 
	}
	
	tokenptr flagt=NULL;
	tokenptr prev;
	tokenptr temp;
	tokenptr t;
	tokenptr identi;
	do{

		temp = getNextToken(fa, &l, &c);
		if(temp==NULL) break;
		if(temp->ln[0]=='{') countC++;
		if(temp->ln[0]=='}') countC--;
		
		if(temp && temp->type==12){ 
			flagt=temp;
			do{
				int count=0;
				prev=temp;
				temp=getNextToken(fa,&l,&c);
				if(temp->type==7){ 
					identi=temp;
					count++;
				}
				if(temp->type==3 || temp->type==11){
					tokenptr temp3;
					if(temp->ln[0]=='['){
						int no=0;
						temp3=getNextToken(fa,&l,&c);
						if(temp3->ln[0]=']'){
							int c3=0;          
							while(temp3->ln[0]!='}'){
								if(temp3->ln[0]==',') c3++;
								temp3=getNextToken(fa,&l,&c);
							}
							temp3=getNextToken(fa,&l,&c);
							no=c3+1;
						}
						else if(temp3->type==5){  
							sscanf(temp3->ln, "%d", &no);
						}
						int str;
						str=getSize(flagt->ln);
						str=str*no;
						char ch;
						if(countC==0) ch='G';
						else ch='L';
						insert(identi->ln,flagt->ln,str,ch,-1,"----");
						if(temp3->ln[0]==';') break;
					}

					if(temp->ln[0]==','){
						int str;
						str=getSize(flagt->ln);
						char ch;
						if(countC==0) ch='G';
						else ch='L';
						insert(identi->ln,flagt->ln,str,ch,-1,"----");
						continue;
					}
					else if(temp->ln[0]=='='){ 
						do{
							t=getNextToken(fa,&l,&c);
						}while(t->ln[0]!=','&&t->ln[0]!=';');
						int str;
						str=getSize(flagt->ln);
						char ch;
						if(countC==0) ch='G';
						else ch='L';
						insert(identi->ln,flagt->ln,str,ch,-1,"----");
					}
					else if(temp->ln[0]=='('){
						int lol=0;
						do{
							temp=getNextToken(fa,&l,&c);
							if(temp->type==12) lol++;
						}while(temp->ln[0]!=')');
						insert(identi->ln,"FUNC",-1,'A',lol,flagt->ln);
						break;
					}
					else if(temp->ln[0]==';'){
						int str;
						str=getSize(flagt->ln);
						char ch;
						if(countC==0) ch='G';
						else ch='L';
						insert(identi->ln,flagt->ln,str,ch,-1,"----");
						break;
					}
				}
			}while(temp && (temp->ln[0]!=';'));
		}
	
	}while(temp!=NULL);
	display();
}
