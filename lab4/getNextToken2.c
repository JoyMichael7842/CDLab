#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

typedef struct TOKEN
{
    int row, col;
    char type[10];
    char lexeme[10];
}*Token; 

Token getNextToken(FILE *fp){

//    Token token = (Token)malloc(sizeof(Token));
    char ca;
    ca = getc(fp);
//    char s = ca;
    printf("%c",ca);
//    return 
}

int main(){
    FILE *fp;
    fp = fopen("input.c","r");
    for(int i=0;i<2;i++){
        getNextToken(fp);
    }
}