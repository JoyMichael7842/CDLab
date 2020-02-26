#include<stdio.h>
#include<stdlib.h>
#include<string.h>



struct entry
{
	int index ; // index of the entry
	char name[50];
	char type[4];
	int size ;
	char scope ;
	int noa ; // no of arguments
	char returntype[50];
	int argument; // index of the argument of the function in the table

};

void display(struct entry arr[50] , int size)
{
	printf("Index    Name  Type   Size  Scope  No.of Argument    Arguments   Return\n");
	for(int i = 1 ;i<=size;i++)
	{

		printf("%d\t",arr[i].index);
		printf("%s\t",arr[i].name);
		if(strcmp(arr[i].name,"printf")==0 || strcmp(arr[i].name,"scanf")==0)
		{
		printf("--\t");
		printf("0\t");
		}
		else
		{
			printf("%s\t",arr[i].type);
		printf("%d\t",arr[i].size);
		}
		
		printf("%c\t",arr[i].scope);
		printf("%d\t",arr[i].noa);
		if(arr[i].argument!=0 && arr[i].argument!=-1)
		printf("\t<id,%d>\t",arr[i].argument);
	else
			printf("\t\t");
				printf("%s\t",arr[i].returntype);
		printf("\n");

	}
}

int variableAlreadyExists(char *buf, struct entry arr[50], int tablesize)
{
	for(int i=0;i<tablesize;i++)
	{
		if(strcmp(buf,arr[i].name)==0)
		{
			//fprintf(output,"< id,%d >",arr[i].index);
			return i;
		}
	}
	return 0;
}

int main()
{

	FILE * fa , *fb,*output ;
	int tablesize = 1 ; // no of entries in the symbol table
	struct entry arr[50];
	char keyword[10][50]={"int","double","long","short","float","char","void","if","else","return"};
	int size[10]={4,8,8,2,4,1,0,0,0,0};
	char ca , cb,temp ;
	int i,x=0 ;
	fa = fopen("testcase.c","r");
	output=fopen("Out.txt","w");
	char a[100];
	ca = getc(fa);
	char buf[50];
	char na[3]="NA";
	char keybuf[50]; // will contain the last keyword found
	int icheck = 0 ; // this is set to 1 if an identifier has been found
	//int currindex = 0 ; //used to keep track of symbol table index of function which has been found
	int lastkeywordindex = 0 ; // keep track of index of last keyword in the keyword array
	int lastfuncindex = 0 ; // keep track of index of last function
	int insidefuncparam = 0 ; // if set we are inside function parameter list
	int funcbodystart = 0 ; // if set we are inside the function body
	while(ca!=EOF)
	{
		//printf("%c",ca);
		if(icheck==1 && ca!=' ' && ca!='\n' &&  ca!='(' && insidefuncparam==0 && funcbodystart==0)
		{
			// global variable found 

			struct entry * item = malloc(sizeof(struct entry));
			item->index = tablesize ;
			tablesize ++ ;
			strcpy(item->name,buf);
			strcpy(item->type,keybuf);
			item->size = size[lastkeywordindex];
			item->scope = 'G';
			item->noa = 0 ;
			item->argument = -1 ;
			strcpy(item->returntype,na);
			arr[item->index] = (*item) ;
			icheck=0;
			fprintf(output,"< id,%d >",item->index);
		}


		if(ca == '"')
		{
			
			ca = getc(fa);
			while(ca!='"')
			{
				a[x]=ca;
			    x++;
				ca = getc(fa);
			}
			fprintf(output,"<\" %s \">",a);

			x=0;
			
		}

		if(ca == '#')
		{
			ca = getc(fa);
			while(ca!='\n')
				ca = getc(fa);
		
		}
		if (ca=='/')
		{
			cb = getc(fa);
			if (cb == '/')
			{
				while(ca != '\n')
					ca = getc(fa);
			}
			else if (cb == '*')
			{
				do
				{
					while(ca != '*')
						ca = getc(fa);
						ca = getc(fa);
				} while (ca != '/');
			}
			
		}

		if((ca>=(int)'a' && ca<=(int)'z') || (ca>=(int)'A' && ca<=(int)'Z')||(ca=='_'))// either a keyword or an identifier
		{
			memset(buf,'\0',sizeof(buf) * sizeof(char));
			int keywordOrIdentifier = 0 ;
			//int res = 0 ;
			

			buf[0]=ca ; int pos = 1;
			cb = getc(fa);
			//printf("%c ", ca);

			while((cb>=(int)'a' && cb<=(int)'z') || (cb>=(int)'A' && cb<=(int)'Z') || (cb>=(int)'0' && cb<=(int)'9')||(cb=='_'))
			{
				//check = 1 ;
				buf[pos++]=cb ;
				cb = getc(fa);

				//printf("%c ", cb);
			}
			//fseek(fa, -1, SEEK_CUR);
			buf[pos]='\0';
			for(i = 0 ;i<10;i++)
			{
				if(strcmp(buf,keyword[i])==0)
				{
					keywordOrIdentifier = 1 ;//keyword has been found
					//res = i ;
					break ;
				}
			}


			if(keywordOrIdentifier==0)// keyword not found . Identifier has been found
			{
				
				icheck = 1 ;
				if(funcbodystart==0 && insidefuncparam==0)//outside function body and outside function params
				{
				}
				else if((insidefuncparam==0 && funcbodystart==1) || (insidefuncparam==1 && funcbodystart==0) )//a variable either in the body of a function or a parameter of the function
				{
					// local variable has been found

					if(icheck=variableAlreadyExists(buf, arr, tablesize))//dont insert this variable into symbol table if it already exists there
					{
						fprintf(output,"< id,%d >",arr[icheck].index);
						icheck = 0 ;
						ca = getc(fa);
						continue;
					}


					struct entry * item = malloc(sizeof(struct entry));
					item->index = tablesize ;
					tablesize ++;
					strcpy(item->name,buf);//name of the variable
					strcpy(item->type,keybuf);//name of the data type that was found just before this variable was found
					item->size = size[lastkeywordindex];
					item->scope = 'L';
					item->noa = 0 ;
					item->argument = -1 ;
					strcpy(item->returntype,na);
					fprintf(output,"< id,%d >",item->index);




					arr[item->index] = (*item) ;// as arr[tablesize-1]


					if(insidefuncparam==1)// if it is a parameter of the function
					{
						arr[lastfuncindex].argument = item->index;
						arr[lastfuncindex].noa = 1 ;
					}
					icheck = 0;
				}
				

			}
			else// keyword has been found
			{
				icheck = 0 ;
				lastkeywordindex = i ;
				strcpy(keybuf,buf) ; // now keybuf contains
				fprintf(output,"< %s >",keybuf);
				// keyword has been found
			}
			
			
				ca = cb;
				continue ;
			

		}
		else if(ca==' ')
		{

		}
		else if(ca=='(' && icheck==1)
		{
			// a fn has been found . The value in buffer is the name of the function
			struct entry * item = malloc(sizeof(struct entry));
			item->index = tablesize ;
			tablesize ++ ;

			strcpy(item->name,buf);
			char type[]="FUNC";
			strcpy(item->type,type);
			item->scope = 'G' ;
			item->argument = -1 ;
			strcpy(item->returntype,keyword[lastkeywordindex]);
			insidefuncparam = 1 ;
			lastfuncindex = item->index ;

			arr[item->index] = (*item) ;
			icheck = 0 ;
			fprintf(output,"< id,%d >< ( >",item->index);

		}
		else if(ca==')' && insidefuncparam==1)
		{
			insidefuncparam = 0 ;
			fprintf(output,"< ) >\n");
		}
		else if(ca==')')
		{
			fprintf(output,"< ) >");
		}
		else if(ca=='{')
		{
			funcbodystart = 1 ;
			fprintf(output,"< {>\n");
		}
		else if(ca=='}')
		{
			funcbodystart = 0 ;
			fprintf(output,"< } >\n");
		}
		else if(ca=='(')
		{
			fprintf(output,"< ( >");
		}
	   
	    else if(ca==';')
		{
			fprintf(output,"< ; >\n");
		}
		

		if(ca=='+'||ca=='-'||ca=='/'||ca=='*'||ca=='%')
		{
		a[x++]=ca;
		ca=getc(fa);
		if(ca=='='){
			a[x++] = ca;
			
			a[x++] = '\0';
			fprintf(output,"< %s >",a);
			
		}
		else{
			a[x++]='\0';
			fseek(fa, -1, SEEK_CUR);
			fprintf(output,"< %s >",a);

		}
		x=0;
	}

	if(ca == '<' || ca == '>' || ca == '!' || ca == '='){
		a[x++]=ca;

		ca = getc(fa);
		if(ca=='='){
			a[x++] = ca;
			
			a[x++] = '\0';
			fprintf(output,"< %s >",a);
			x=0;
		}
		else{
			a[x++]='\0';
			fseek(fa, -1, SEEK_CUR);
			fprintf(output,"< %s >",a);
			x=0;
		}
	}
	if(ca >= '0' && ca <= '9')
	{

		fprintf(output,"< num,%c >",ca);	
	}
	if(ca == ',')
	{

		fprintf(output,"< , >");	
	}
	

		ca = getc(fa);
	}
	display(arr,tablesize-1);
	

}
