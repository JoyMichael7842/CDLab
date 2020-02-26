#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define TableLength 30
char strings[20][20];
int j=0;
enum tokenType{EOFILE=-1,LESS_THAN,LESS_THAN_OR_EQUAL,GREATER_THAN_OR_EQUAL,EQUAL,NOT_EQUAL};

struct token
{
	char *lexeme;
	char type[10];
	int size;
	char scope;
	int n_arg;
	char arg[10];
	char rtype[10];
};


int func(char str[])
{
	for(int i=0;i<j;i++)
		if(strcmp(str,strings[i])==0)
			return i+1;
	return 0;
}

struct ListElement
{
	struct token tok;
	struct ListElement *next;
};

struct ListElement *TABLE[TableLength];

void Initialize()
{
	for(int i=0;i<TableLength;i++)
		TABLE[i]=NULL;
}

// void disp(struct token tk)
// {
// 	printf("%s\t%s\t%d\t%c\t%d\t%s\t%s\n",tk.lexeme,tk.type,tk.size,tk.scope,tk.n_arg,tk.arg,tk.rtype);
// }
void Display()
{
	//iterate through the linked list and display
	FILE *fb = fopen("q1_symbol.txt","w");
	struct ListElement *ele;
	for(int i=0;i<TableLength;i++)
	{
		if(TABLE[i])
		{
			ele=TABLE[i];
			while(ele!=NULL)
			{
				fprintf(fb,"%s\t%s\t%d\t%c\t%d\t%s\t%s\n",ele->tok.lexeme,ele->tok.type,ele->tok.size,ele->tok.scope,ele->tok.n_arg,ele->tok.arg,ele->tok.rtype);
				ele=ele->next;
			}
		}
	}
	fclose(fb);
}

int HASH(char *str)
{
	//Develop an OPENHASH function on a string
	int s=0;
	for(int i=0;i<strlen(str);i++)
		s+=str[i];
	return s%TableLength;
}

int SEARCH(char* str)
{
	//Write a search routine to check whether a lexeme exists in Symbol table
	//Returns 1 if lexeme is found
	//else returns 0
	struct ListElement *ele;
	int val = HASH(str);

	if(TABLE[val]!=NULL)
	{
		ele=TABLE[val];
		while(ele!=NULL)
		{
			if(strcmp(ele->tok.lexeme,str)==0)
					return 1;
			ele=ele->next;
		}
	}
	return 0;
}

void INSERT(struct token tk)
{
	if(SEARCH(tk.lexeme)==1)
	{
		return;
	}

	int val = HASH(tk.lexeme);
	struct ListElement* cur= (struct ListElement*)malloc(sizeof(struct ListElement));
	cur->tok = tk;
	cur->next=NULL;

	if(TABLE[val] == NULL)
		TABLE[val]=cur; 
	else
	{
		struct ListElement* ele = TABLE[val];
		while(ele->next!=NULL)
			ele=ele->next;
		ele->next=cur;
	}
}

int main()
{
	char c,p,buf[10];
	bool func_start=0;
	int i,k,ct=0,flag,ni=0,nn=0;
	FILE *fp = fopen("test2.c","r");
	FILE *fc = fopen("q1_out.txt","w");
	
	c = fgetc(fp);
	
	char str1[10],str2[10],prev_str[10];
	struct token tk,ts;
	strcpy(str2,"return");
	strcpy(str1,"int");
	char str[100];

	while(c!=EOF)
	{
		flag=0;
		i=0;
		
		if(c=='{')
			fprintf(fc,"<{>");
		else if(c=='}')
			fprintf(fc,"<}>");
		else if(c==')')
			fprintf(fc,"<)>");
		else if(c=='(')
			fprintf(fc,"<(>");
		else if(c=='*')
			fprintf(fc,"<*>");
		else if(c=='-')
			fprintf(fc,"<->");
		else if(c=='=')
			fprintf(fc,"<=>");
		else if(c==';')
			fprintf(fc,"<;>");
		else if(c=='>')
			fprintf(fc,"<>>");
		while(c>='0' && c<='9')
		{
			str[i++]=c;
			flag=2;
			c=fgetc(fp);
		}

		if(flag==2)
		{
			nn++;
			fprintf(fc," <num,%d> ",nn);
		}

		while((c>='A'&& c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9')|| c=='_')
		{
			str[i++]=c;
			c=getc(fp);
			flag=1;
		}
		if(flag==1)
		{
			str[i]='\0';
			//printf("%s9\n",str);
			tk.lexeme = (char*)malloc((strlen(str)+1)*sizeof(char));
			if(strlen(str)>0)
			{
				if(strcmp(str1,str)==0)
				{
					strcpy(prev_str,str);
					ni++;
					fprintf(fc, "<%s> <id,%d>",str1,ni);
				}
				else if(strcmp(str2,str)==0)
				{
					fprintf(fc,"<%s>",str2);
				}
				else if(strcmp("if",str)==0 || strcmp("else",str)==0 || strcmp("printf",str)==0)
				{
					fprintf(fc," <%s> ",str);
				}
				else if(func(str)==0)
				{
					strcpy(strings[j++],str);
					strcpy(tk.lexeme,str);
					while(c==' ')
						c=fgetc(fp);
					if(c=='(')
					{
						fprintf(fc,"<(>");
						prev_str[strlen(prev_str)]='\0';
						//strcpy(tk.rtype,prev_str);
						printf("%s\n",prev_str);
						strcpy(tk.type,"FUNC");
						func_start = !func_start;
						tk.size=0;
						tk.scope='G';
						c=fgetc(fp);
						while(c==' ')
							c=fgetc(fp);
						if(c==')')
						{
							tk.n_arg=0;
							fprintf(fc,"<)>");
							strcpy(tk.arg,"");
							// disp(tk);
							INSERT(tk);
						}
						else
						{
							char st[10];k=0;
							while(((c>='A'&& c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9')|| c=='_'))
							{
								st[k++]=c;
								c=fgetc(fp);
							}
							k=0;
							while(c==' ')
								c=fgetc(fp);
							while(((c>='A'&& c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9')|| c=='_'))
							{
								st[k++]=c;
								c=fgetc(fp);
							}
							st[k]='\0';
							if(strlen(st)>0)
							{
								//printf("%s YOLO\n",st);
										ts.lexeme = (char*)malloc((strlen(st)+1)*sizeof(char));
										strcpy(ts.lexeme,st);
										strcpy(ts.type,"int");
										ts.size=4;
										ts.scope=' ';
										ts.n_arg=0;
										strcpy(ts.arg,"");
										strcpy(ts.rtype,"");
										// disp(ts);
										INSERT(ts);
										strcpy(strings[j++],st);
										k=0;
							}
							ni++;
							strcpy(tk.arg,"<int> <id,");
							char end[2];end[0]=48+ni;end[1]='\0';
							strcat(tk.arg,end);
							strcat(tk.arg,"> ");
							tk.n_arg=1;
							fprintf(fc,"%s",tk.arg);
							fprintf(fc, "<)>");
							// disp(tk);
							INSERT(tk);
						}
					}
					else if(c==';')//Semicolon
					{
						strcpy(tk.lexeme,str);
						strcpy(tk.type,"int");
						tk.size=4;
						if(func_start==1)
							tk.scope='L';
						else
							tk.scope='G';
						tk.n_arg=0;
						strcpy(tk.arg,"");
						strcpy(tk.rtype,"");
						//disp(tk);
						INSERT(tk);
						fprintf(fc, "<;>");
					}
				}
				else if(func(str))
				{
					fprintf(fc, "<id,%d>",func(str));
				}
			}
		}
		else if(flag!=2)
		{
			c=fgetc(fp);
			while(c=='\n' || c==' ')
			{
				if(c=='\n')
					fprintf(fc,"\n");
				c=fgetc(fp);
			}
		}
	}
	Display();
	return 0;
}