#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

char keyword[11][50]={"int","return","void","char","printf","if","else","break","for","do","continue"};


typedef struct TOKEN
{
    int row, col;
    char  type[100];
    char lexeme[100];
}*Token; 

void printToken(Token token){
    printf("%s %s\n",token->type,token->lexeme);
}

Token getNextToken(FILE *fp){

    Token token = (Token)malloc(sizeof(struct TOKEN));
    char ca,cb;
    int i;
    ca = getc(fp);
    if(ca==EOF)
        return NULL;
    char buf[100];
    if((ca>='a' && ca<='z')||(ca>='A' && ca<='Z')){
        i=0;
        buf[i] = ca;
        cb=ca;
        while((cb>='a' && cb<='z')||(cb>='A' && cb<='Z')){
            cb = getc(fp);
            buf[++i] = cb; 
        }
        buf[i] = '\0';
        fseek(fp, -1, SEEK_CUR);
        for(i=0;i<11;i++){
            if(strcmp(buf,keyword[i])==0){
                strcpy(token->lexeme,buf);
                strcpy(token->type,"KEYWORD");
            }
            else{
                strcpy(token->lexeme,buf);
                strcpy(token->type,"IDENTIFIER");
            }
        }
        return token;
    }

    else if(ca>='0' && ca <='9'){
        cb = ca;
        i=0;
        buf[i] = cb;
        while(cb>='0' && cb<='9'){
            cb = getc(fp);
            buf[++i] = cb;
        }
        buf[i] = '\0';
        fseek(fp, -1, SEEK_CUR);
        strcpy(token->lexeme,buf);
        strcpy(token->type,"NUMBER");
        return token;
    }

    else if(ca == '*'||ca == '+'||ca == '-'||ca == '/'||ca == '['||ca == ']'||ca == '{'||ca == '}'||ca == ';'||ca == '/'||ca == '[')

    else if(ca=='\n'||ca=='\t'||ca == ' '){
        printf("%c",ca);
        getNextToken(fp);

    }
    
}

int main(){

    Token token;
    FILE *fp;
    fp = fopen("input2.c","r");
    
    token = getNextToken(fp);
    printToken(token);
    token = getNextToken(fp);
    printToken(token);
}