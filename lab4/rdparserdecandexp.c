#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

Token lookahead;
Table t;
FILE *fp;
int status=1;
int i = 0;

void proc_declarations();
void proc_datatype();
void proc_identifier_list();
void proc_identifier_list_1();
void proc_identifier_list_2();
void proc_statement_list();
void proc_statement();


void proc_program()
{
	if(strcmp(lookahead->lexeme,"main")==0){
		printf("%s\n",lookahead->lexeme);
		lookahead = getNextToken(fp,&t);
		if(strcmp(lookahead->lexeme,"(")==0){
			printf("%s\n",lookahead->lexeme);
			lookahead = getNextToken(fp,&t);
		}
		else{
			status = 0;
			return;
		}
		if(strcmp(lookahead->lexeme,")")==0){
			printf("%s\n",lookahead->lexeme);
			lookahead = getNextToken(fp,&t);
		}
		else{
			status = 0;
			return;
		}
		if(strcmp(lookahead->lexeme,"{")==0){
			printf("%s\n",lookahead->lexeme);
			proc_declarations();
			//proc_statement_list();
			return;
		}
		else{
			status = 0;
			return;
		}
		
	}
	else{
		status = 0;
		return; 
	}
}

void proc_declarations(){
	proc_datatype();
	proc_identifier_list();
	return;
}

void proc_datatype(){
	lookahead = getNextToken(fp,&t);
	printf("%s\n",lookahead->lexeme);
	if((strcmp(lookahead->lexeme,"int")==0) || (strcmp(lookahead->lexeme,"char")==0)){
		return;
	}
	else{
		status = 0;
		return;
	}
}

void proc_identifier_list(){
	lookahead = getNextToken(fp,&t);
	printf("%s\n",lookahead->lexeme);
	if(lookahead->type == IDENTIFIER){
		proc_identifier_list_1();
		return;
	}
	else{
		status = 0;
		return;
	}
}

void proc_identifier_list_1(){
	lookahead = getNextToken(fp,&t);
	printf("%s\n",lookahead->lexeme);
	printf("hii");
	if(strcmp(lookahead->lexeme,",")==0){
		proc_identifier_list();
	}
	else if(strcmp(lookahead->lexeme,"[")==0){
		lookahead = getNextToken(fp,&t);
		printf("%s\n",lookahead->lexeme);
		if(lookahead->type==NUMBERICAL_CONSTANT){
			lookahead = getNextToken(fp,&t);
			if(strcmp(lookahead->lexeme,"]")==0){
				proc_identifier_list_2();
			}
			else{
				status = 0;
				return;
			}
		}
		else{
			status = 0;
			return;
		}
	}
	else if(strcmp(lookahead->lexeme,";")==0){
		return;
	}
}

void proc_identifier_list_2(){
	lookahead = getNextToken(fp,&t);
	printf("%s\n",lookahead->lexeme);
	if(strcmp(lookahead->lexeme,",")==0){
		proc_identifier_list();
	}
	else if(strcmp(lookahead->lexeme,";")==0){
		return;
	}
}

void proc_statement_list(){
	lookahead = getNextToken(fp,&t);
	printf("%s\n",lookahead->lexeme);
	if(strcmp(lookahead->lexeme,"}")==0){
		return;
	}
	else{
		proc_statement();
		return;
	}

}


void proc_statement(){

}

int main(){
	fp = fopen("input.c","r");
	lookahead = getNextToken(fp,&t);
	proc_program();
	lookahead = getNextToken(fp,&t);
	printf("%s\n",lookahead->lexeme);
	
	if(strcmp(lookahead->lexeme,"$")==0 && status){
		printf("Sucessful\n");
	}
	else{
		printf("error\n");
	}

	return 0;
}