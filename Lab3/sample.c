#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TableLength 30
#define STRLEN 10
enum tokenType {EOFILE=-1,LESS_THAN,LESS_THAN_OR_EQUAL,
GREATER_THAN,GREATER_THAN_OR_EQUAL,EQUAL,NOT_EQUAL};

int l,c,id=0;

struct token{
	char lexeme[STRLEN];
	int index;
	unsigned int rowno,colno;
	enum tokenType type;
};

struct listElement{
	struct token tok;
	struct listElement *next;
};

struct listElement *TABLE[TableLength];

void intialize(){
	for(int i = 0; i  < TableLength ;++i)
		TABLE[i] = NULL;
}

void display(){
	for(int i = 0 ; i <  TableLength ; ++i){
		struct listElement* tempEle = TABLE[i];
		while(tempEle != NULL){
			struct token tempToken = tempEle->tok;
			printf("%s%s\n","Name : ",tempToken.lexeme );
			printf("%s%d\n","Index : ",tempToken.index);
			printf("%s%d\n","Row num : ",tempToken.rowno );
			printf("%s%d\n","Column num : ",tempToken.colno );
			printf("%s%d\n","Row num : ",tempToken.rowno );
			printf("%s%d\n","Token Type : ",tempToken.type );
			printf("%s\n","-------------------------------------------------" );
			tempEle = tempEle->next;
		}
	}
}


int hash(char *str){
	int sum = 0;
	for(int i = 0 ; i < strlen(str) ; ++i)
		sum = sum + (int)str[i];
	return sum%TableLength;
}

int search(char *str){
	for(int i = 0 ; i <  TableLength ; ++i){
		struct listElement* tempEle = TABLE[i];
		while(tempEle != NULL){
			struct token tempToken = tempEle->tok;
			if(strcmp(tempToken.lexeme,str) == 0)
				return 1;
			tempEle = tempEle->next;
		}
	}
	return 0;
}

void insert(struct token tk){
	if(search(tk.lexeme) == 1)
		return;

	int val = hash(tk.lexeme);
	//printf("%d\n", val);
	struct listElement *current = (struct listElement*)malloc(sizeof(struct listElement));
	current->tok = tk;
	current->next = NULL;
	current->tok.index = id;
	if(TABLE[val] == NULL)
		TABLE[val] = current;
	else{
		struct listElement *tempEle = TABLE[val];
		while(tempEle->next != NULL){
			tempEle = tempEle->next;
		}
		tempEle->next =  current;
	}
	id++;
}

int main(){
	FILE *fa,*fb;
	l=1, c=1;
	char ch;
	fa = fopen("test.c", "r");
	
	if (fa == NULL){
		printf("Cannot open file \n");
		exit(0); 
	}	

	ch = getc(fa);	
	while(ch != EOF){
		//printf("%c\n",ch );
		if(ch == '\n'){
			l++;
			c = 1; 
		}
		
		else if(ch == ' '){
			c++;
		}

		else if(ch == '<' || ch == '>' || ch == '!' || ch == '='){
			int i = 0;
			char a[5];
			struct token newToken;
			newToken.rowno = l;
			newToken.colno = c;
			c++;
			a[i] = ch;
			i++;
			ch = getc(fa);
			if(ch=='='){
				a[i] = ch;
				i++;
				c++;
				a[i] = '\0';
				strcpy(newToken.lexeme, a);

				if(a[0] == '<')
					newToken.type = LESS_THAN_OR_EQUAL;
				else if(a[0] == '>')
					newToken.type = GREATER_THAN_OR_EQUAL;
				else if(a[0] == '=')
					newToken.type = EQUAL;
				else
					newToken.type = NOT_EQUAL;
				insert(newToken);
			}
			else{
				a[i]='\0';
				if(a[0] == '<')
					newToken.type = LESS_THAN;
				else if(a[0] == '>')
					newToken.type = GREATER_THAN;
				strcpy(newToken.lexeme, a);

				fseek(fa, -1, SEEK_CUR);
				insert(newToken);
			}
		}


	else{
		c++;
	}

	ch = getc(fa);	
	}
	display();
}