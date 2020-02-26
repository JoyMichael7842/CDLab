#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NEWLINE '\n'
#define STAR '*'
#define SPACE ' '
#define TAB '\t'
enum Type {ARITHMETIC_OPERATOR, LOGICAL_OPERATOR, RELATIONAL_OPERATOR, SPECIAL_SYMBOL, KEYWORD, NUMBERICAL_CONSTANT, STRING, IDENTIFIER, CHARACTER_CONSTANT, UNKNOWN};

int nlf = 0, spf = 0;
typedef struct TOKEN
{
    int row, col;
    enum Type type;
    char lexeme[10];
}*Token;

int size(char *s)
{
    if(strcmp(s, "int")==0)return sizeof(int);
    if(strcmp(s, "unsigned int")==0)return sizeof(unsigned int);
    if(strcmp(s, "long")==0)return sizeof(long);
    if(strcmp(s, "short")==0)return sizeof(short);
    if(strcmp(s, "char")==0)return sizeof(char);
    if(strcmp(s, "double")==0)return sizeof(double);
    if(strcmp(s, "float")==0)return sizeof(float);
    if(strcmp(s, "void")==0)return sizeof(void);
}

int count=0, SCOPE = 0, BRACKET = 0, FUNC = 0, DEF=0, ARR =0, FCOUNT=0;
char TYPE[10];

typedef struct TABLE
{
    int id;
    int size;
    char scope;
    int nargs;
    char argid[30];
    char idname[10];
    char type[10];
    char rettype[10];
    struct TABLE *next;
}*Table;

int hash(char *s)
{
    int val;
    if(s[0] == '_')val = 0;
    else if(s[0]>='a')val = s[0] - 'a' + 1;
    else val = s[0] - 'A' + 1;
//    printf("Inside hash\n");
    return val;
}

int search(Table table, char *s)
{
//    printf("Inside search\n");
    Table t = table;
    while(t!=NULL)
    {
        //printf("%d\t%s\t%s\t%d\t%s\t%c\t%d\t%s\n", t->id, t->idname, t->type, t->size, t->rettype, t->scope,t->nargs, t->argid);
        if(strcmp(s, t->idname) == 0)
            return 1;
        t = t->next;
    }
//    printf("Exiting search\n");
    return 0;
}
void print(Table t)
{
    printf("%d\t%s\t%s\t%d\t%s\t%c\t%d\t%s\n", t->id, t->idname, t->type, t->size, t->rettype, t->scope,t->nargs, t->argid);
}
void printTable(Table *table)
{
    for(int i =0; i<27;i++)
    {
        Table t = table[i];
        while(t!=NULL)
        {
            print(t);
            t = t->next;
        }
    }
}

Table findbyid(Table *table, int id)
{
    int i;
    Table t;
    for(i=0;i<27;i++)
    {
        t = table[i];
        while (t!=NULL)
        {
            if(t->id == id)
            return t;
            t = t->next;
        }
    }
    return NULL;
}
int argcnt = 0;
int ROW = 1, COL = 1, ISSTRING = 0;
int ELECOUNT = 1;
void insert(Table *table, Token token)
{
//    printf("Before hash\n");
    int val = hash(token->lexeme);
    if(search(table[val], token->lexeme))
    {
        return;
    }
//    printf("INSIDE INSERT\n");
    Table temp = malloc(sizeof(struct TABLE));
    Table t = table[val], cur=NULL;
    while(t!=NULL)
    {
        cur = t;
        t=t->next;
    }
//    printf("%s, %s\n",token->lexeme, TYPE);
//   printf("Got t\n");
    if(cur!=NULL)
    cur->next = temp;
    else table[val] = temp;
    temp->id = ++count;
    strcpy(temp->idname, token->lexeme);
    if(FUNC && DEF==0)
    {
        strcpy(temp->type, "FUNC");
        strcpy(temp->rettype, TYPE);
//        temp->nargs = 0;
        argcnt = 0;
    }

    else 
    {
        strcpy(temp->type, TYPE);
        temp->size = size(TYPE);
        if(BRACKET == 0 && !DEF)
        {
            temp->scope = 'G';
        }
        else
            temp->scope = 'L';
        if(DEF == 1)
        {
            argcnt++;
            Table c = findbyid(table, temp->id-argcnt);
            char ch[8] = "<id, ";
            ch[5] = temp->id+'0';
            ch[6] = '>';
            ch[7] = '\0';
            c->nargs = argcnt;

           strcat(c->argid, ch);
 //          printf("%s", ch);
        }
    }
//    printf("Assigned\n");
//      printf("%d\n", count);
}

int getid(Table *table, Token token)
{
    int val = hash(token->lexeme);
    Table t = table[val];
    while(t!=NULL)
    {
        if(strcmp(t->idname, token->lexeme)==0)
            return t->id;
        t=t->next;
    }
}

void printToken(Token token, Table *table)
{
    if(token->lexeme[0]!= SPACE && token->lexeme[0]!= NEWLINE && token->lexeme[0]!= TAB)
    {
        printf("( %s, %d, %d,",token->lexeme, token->row, token->col);
    
        switch(token->type)
        {
            case ARITHMETIC_OPERATOR : printf("ARITHMETIC_OPERATOR");break;
            case LOGICAL_OPERATOR :printf("LOGICAL_OPERATOR");break;
            case RELATIONAL_OPERATOR :printf("RELATIONAL_OPERATOR");break;
            case SPECIAL_SYMBOL :printf("SPECIAL_SYMBOL");break;
            case KEYWORD :printf("KEYWORD");break;
            case NUMBERICAL_CONSTANT :printf("NUMERICAL_CONSTANT");break;
            case STRING :printf("STRING");break;
            case IDENTIFIER :printf("IDENTIFIER");
                             int id = getid(table, token);
                             printf("<id, %d > ", id);
                                break;
            case CHARACTER_CONSTANT : printf("CHARACTER_CONSTANT");break;
            default  : printf("UNKNOWN");
        }
        printf(" )\n");
    }
}

void writeTokenToFile(FILE *f, Token token, Table *table)
{
    if(token->lexeme[0]!= SPACE && token->lexeme[0]!= NEWLINE && token->lexeme[0]!= TAB)
    {
        fputc('<', f);
        if(token->type == IDENTIFIER)
        {
            fprintf(f, "id, %d", getid(table, token));
        }
        else
            fprintf(f, "%s", token->lexeme);
        fputc('>', f);
    }
    else
        fprintf(f, "%s", token->lexeme);
    if(spf)
    {
        spf = 0;
        fputc(' ', f);
    }
    if(nlf)
    {
        nlf = 0;
        fputc( '\n', f);
    }
//    if(token->lexeme[0]=='{')BRACKET++;
//    if(token->lexeme[0]=='}')BRACKET--;
//printf("%s\n", token->lexeme);
}

int isDelimiter(char c)
{
    switch(c)
    {
        case '(':
        case '}':
        case '{':
        case ')': 
        case ' ':
        case '\n':
        case ':':
        case '[':
        case ']':
        case '<':
        case '>':
        case '=':
        case ';':
        case ',':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '&':
        case '^':
        case '!':
        case '|':
 //       case '.':
        case EOF:
        case '$':
        case '?':
        case '\'': return 1;
        default: return 0;
    }
}

//checks if string is keyword
int isKeyword(char a[])
{
    char keywords[][10] = {"for", "if", "while", "do", "struct", "return", "switch", "case", "else"};
    char types[][15] = { "int", "char", "long", "short", "float", "void", "unsigned int", "double"};
    for(int i = 0; i< 9; i++)
    {
        if(strcmp(keywords[i], a)==0)
            return 1;
    }
    for(int i = 0; i< 8; i++)
    {
        if(strcmp(types[i], a)==0)
        {
            strcpy(TYPE, types[i]);
            return 1;
        }
    }
    return 0;
}

//checks if string is a number
int isNumber(char a[])
{
	int i, point = 0;
	for(i=0;a[i]!='\0';i++)
	{
		if(a[i]!='.' && (a[i]<'0'||a[i]>'9'))
			return 0;
		if(a[i] == '.')
		{
			if(point)
			{
				return 0;
			}
			point++;
		}
	}
	return 1;
}
//checks if string is an identifier
int isIdentifier(char a[])
{
	int i;
	if(!(a[0]=='_' || a[0] >= 'a' && a[0] <= 'z' || a[0] >= 'A' && a[0] <= 'Z' ))
		return 0;
	for(i=1; a[i]!='\0';i++)
	{
		if(!(a[i]=='_' || a[i] >= 'a' && a[i] <= 'z' || a[i] >= 'A' && a[i] <= 'Z' || a[i] >='0' && a[i]<='9'))
			return 0;
	}
	return 1;
}


Token getNextToken(FILE *f, Table *table)
{
    Token token = (Token)malloc(sizeof(struct TOKEN));
    char c, buf[32];
    c = fgetc(f);
    if(c==EOF)
        return NULL;
    int i = 0;


    if(!isDelimiter(c))
    {
    	//Check for string, note if beginning of string or end
        if(c == '\"')
        {
            if(ISSTRING == 0)
                ISSTRING = 1;
            else ISSTRING = 0;
            token->row = ROW;
            token->col = COL;
            token->lexeme[0] = c;
            token->lexeme[1] = '\0';
            token->type = SPECIAL_SYMBOL;
        } 
        else
        {
        	//From above, if inside a string, all characters till a '"' are considered part of the string literal
            if(ISSTRING)
            {
                while(c!='\"')
                {
                    buf[i++] = c;
                    c = fgetc(f);
                }
            }
            else
            {
            	//If not string, get the next lexeme
                while(!isDelimiter(c))
                {
                    buf[i++] = c;
                    c = fgetc(f);
                    if(c == '.' && isNumber(buf))
                    {
                    	printf("%d", c);
                    	buf[i++]= c;
                    	c = fgetc(f);
                    }
                }
            }
            buf[i]='\0';
            token->row = ROW;
            token->col = COL;
            strcpy(token->lexeme,buf);

            if(c!=' '&&c != NEWLINE && c != TAB)
            {
                fseek(f, -1, SEEK_CUR);
            }
            else if(c==NEWLINE)
            {
                COL = 1;
                ROW++;
                nlf = 1;
            }
            else
            {
                spf = 1;
                COL++;
            }

            if(ISSTRING)
            {
                if(ARR)
                {
                    ARR = 0;
                    Table c = findbyid(table, count);
                    c->size = size(TYPE) * i;
//                    printf("JIU: %d, %d\n", c->id, c->size);
                }
                token->type = STRING;
            }
            else if(isKeyword(buf))
            {
//                printf("Keyword found : %s\n", buf);
                token->type = KEYWORD;
            }

            else if(isNumber(buf))
            	token->type = NUMBERICAL_CONSTANT;

            else if(isIdentifier(buf))
            {
                token->type = IDENTIFIER;
                c = fgetc(f);
                if(c == '(')
                {
                    FUNC = 1;
                    SCOPE++;
//                    DEF = 1;
                }
//                else if(c == ')' && DEF)DEF=0;
                else FUNC = 0;
                if(c == '[')ARR = 1;
                else ARR = 0;
                fseek(f, -1, SEEK_CUR);
                insert(table, token);
            }
            else
            	token->type = UNKNOWN;

            COL+=i;
            //Since an extra character was read, go back, unless it is a newline or a space
            
        }
    }

    //If it is a delimiter, finding out which delimiter it is
    else
    {
        if(c!=' ' && c != NEWLINE && c != TAB)
        {

        	if(c == '\'')
        	{
        		buf[i++] = c;
        		while((c = fgetc(f))!='\'')
        			buf[i++] = c;
        		buf[i++] = c;
        		buf[i] = '\0';
        		token->row = ROW;
            	token->col = COL;
            	strcpy(token->lexeme,buf);
            	COL+=i;
            	token->type = CHARACTER_CONSTANT;
        	}
        	//Increment or decrement, or normal add/subtract
        	else if(c == '+' || c == '-')
        	{
        		token->lexeme[i++] = c;
        		char c2 = fgetc(f);
        		if(c2== c)
        		{
        			token->lexeme[i++] = c;
        		}
        		else if(c2 == '=')
        		{
        			token->lexeme[i++] = c2;
        		}
        		else
        		{
        			fseek(f, -1, SEEK_CUR);
        		}
        		token->lexeme[i] = '\0';
        		token->type = ARITHMETIC_OPERATOR;
        		token->row = ROW;
            	token->col = COL;
            	COL+=i;	
        	}
        	//Assignment or relational equals
        	else if(c == '=')
        	{
        		token->lexeme[i++] = c;
        		c = fgetc(f);
        		if(c != '=')
        		{
        			fseek(f, -1, SEEK_CUR);
        			token->type = ARITHMETIC_OPERATOR;
        			token->lexeme[i] = '\0';
        		}
        		else
        		{
        			token->lexeme[i++] = c;
        			token->lexeme[i] = '\0';
        			token->type = RELATIONAL_OPERATOR;
        		}
        		token->row = ROW;
            	token->col = COL;
        		COL+=i;
        	}
        	//Other arithmetic operators
        	else if (c == '*' || c == '/' || c == '%')
        	{
	            token->type = ARITHMETIC_OPERATOR;
	            token->lexeme[i++] = c;
	            char c2 = fgetc(f);
	            if(c2 == '=')
        		{
        			token->lexeme[i++] = c2;
        		}
        		else
        		{
        			fseek(f, -1, SEEK_CUR);
        		}
        		token->lexeme[i] = '\0';
        		token->type = ARITHMETIC_OPERATOR;
        		token->row = ROW;
            	token->col = COL;
            	COL+=i;
	        }
	        //Relational not equals, or logical not
	        else if(c=='!')
	        {
	        	token->lexeme[i++] = c;
	        	c = fgetc(f);
	        	if(c == '=')
	        	{
	        		token->lexeme[i++] = c;
	        		token->type = RELATIONAL_OPERATOR;
	        	}
	        	else
	        	{
	        		fseek(f, -1, SEEK_CUR);
	        		token->type = LOGICAL_OPERATOR;
	        	}
	        	token->lexeme[i] = '\0';
	        	token->row = ROW;
	            token->col = COL;
	            COL+=i;
	        }
	        //Other relational operators
	        else if(c == '<'||c == '>')
	        {
	        	token->lexeme[i++] = c;
	        	c = fgetc(f);
	        	if(c == '=')
	        	{
	        		token->lexeme[i++] = c;
	        	}
	        	else
	        	{
	        		fseek(f, -1, SEEK_CUR);
	        	}
	        	token->type = RELATIONAL_OPERATOR;
	        	token->lexeme[i] = '\0';
	        	token->row = ROW;
	            token->col = COL;
	            COL+=i;
	        }
	        //Logical and and or
	        else if(c == '&' || c == '|')
	        {
	        	token->lexeme[i++] = c;
	        	char c2 = fgetc(f);
	        	if(c == c2)
	        	{
	        		token->lexeme[i++] = c;
	        	}
	        	else
	        	{
	        		fseek(f, -1, SEEK_CUR);
	        	}
	        	token->lexeme[i] = '\0';
	        	token->type = LOGICAL_OPERATOR;
	        	token->row = ROW;
	            token->col = COL;
	            COL+=i;
	        }
	        //EXOR
	        else if (c == '^')
	        {
	        	token->lexeme[i++] = c;
	        	token->lexeme[i] = '\0';
	        	token->type = LOGICAL_OPERATOR;
	        	token->row = ROW;
	            token->col = COL;
	            COL+=i;
	        }
	        //A normal special symbol
	        else 
	        {
	        	token->lexeme[i++] = c;
	        	token->type = SPECIAL_SYMBOL;
	        	token->lexeme[i] = '\0';
	        	token->row = ROW;
	            token->col = COL;
	            COL+=i;
                if(c == '[')ARR=1;
                if(c == ',' && ARR){ELECOUNT++;}
                if(c == '{'){BRACKET++;FUNC = 0;}
                if(c == '}')
                {   
                    BRACKET--;
                    if(BRACKET == 0)FUNC =0;
                    if(ARR)
                    {
                        Table c = findbyid(table, count); 
                        c->size = c->size * ELECOUNT;
                        ARR = 0;
                        ELECOUNT = 1;
                    }
                }
                if(c == '(' && FUNC && DEF==0){DEF=1, SCOPE = 1;};
                if(c==')' )DEF=0;
//                if(c == ';')printf("Semicolon\n");
	        }
        }
        //Handling newlines and spaces
        else
        {
            token->lexeme[0] = c;
            token->lexeme[1] = '\0';
            if(c==NEWLINE||c == '\t')
            {
                TYPE[0] = '\0';
                COL = 1;
                ROW++;
                token = getNextToken(f, table);
            }
            else if(c==' '){
                COL++;
                token = getNextToken(f, table);
            }
            
        }
        
    }
    return token;
}