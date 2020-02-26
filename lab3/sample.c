#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TableLength 30

enum tokenType{EOFILE=-1,LESS_THAN,LESS_THAN_OR_EQUAL,GREATER_THAN,GREATER_THAN_OR_EQUAL,EQUAL,NOT_EQUAL};

struct token
{
	char *lexeme;
	int index;
	unsigned int rowno,colno;
	enum tokenType type;
};

struct ListElement{
	struct token tok;
	struct ListElement *next;
};

struct ListElement *TABLE[TableLength];

void initialize(){
	for(int i=0;i<TableLength;i++){
		TABLE[i] = NULL;
	}
}
void Display(){

}
int HASH(char *str){

}
int SEARCH(char *str){
	
}
void INSERT(struct token tk){
	if(SEARCH(tk,lexeme)==1){
		return;
	}
	int val=HASH(tk,lexeme);
	struct ListElement* cur = (struct ListElement*)malloc(sizeof(struct ListElement));
	cur->tok = tk;
	cur->next = NULL;

	if(TABLE[val]==NULL){
		TABLE[val] = cur;
	}	
	else{
		struct ListElement* ele TABLE[val];
		while(ele->next!=NULL){
			ele = ele->next;
		}
		ele->next = cur;
	}
}