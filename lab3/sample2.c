#include<stdio.h>
#include<stdlib.h>
#include<string.h>



struct symbtable
{
	int index ; // index of the symbtable
	char name[50];
	char type[4];
	int size ;
	char scope ;
	int noarg ; // no of arguments
	char returntype[50];
	int argument; // index of the argument of the function in the table

};

void display(struct symbtable arr[50] , int size)
{
	printf("\nIndex\t\tName\t\tType\t\tSize\t\tScope\t\tNo of arg\t\targ index in tab\t\treturn\n");
	for(int i = 0 ;i<=size;i++)
	{
		printf("%d\t\t",arr[i].index);
		printf("%s\t\t",arr[i].name);
		printf("%s\t\t",arr[i].type);
		printf("%d\t\t",arr[i].size);
		printf("%c\t\t",arr[i].scope);
		printf("%d\t\t",arr[i].noarg);
		if (arr[i].argument!=-1)
			printf("<id,%d>\t\t",arr[i].argument);
		else
			printf("%d\t\t",arr[i].argument);
		printf("%s\t\t",arr[i].returntype);
		printf("\n\n");

	}
}

int exists(char *buf, struct symbtable arr[50], int tsize)
{
	for(int i=0;i<tsize;i++)
	{
		if(strcmp(buf,arr[i].name)==0)
			return 1;
	}
	return 0;
}
void output(struct symbtable arr[50] , int tsize){
	FILE * fa , *fb ;
	// no of entries in the symbol table
	int p=-1;
	int ca , cb ;
	int i ;
	fa = fopen("sample.c","r");
	fb= fopen("text.txt", "w");
	ca = getc(fa);
	char buf[50];
	char na[3]="NA";
		char keyword[11][50]={"int","return","void","char","float","if","else","break","for","do","continue"};
	char keybuf[50]; // will contain the last keyword found
	int ident = 0 ; // this is set to 1 if an identifier has been found
	//int currindex = 0 ; //used to keep track of symbol table index of function which has been found
	int lastkeywordindex = 0 ; // keep track of index of last keyword in the keyword array
	int lastfuncindex = 0 ; // keep track of index of last function
	int insidefuncparam = 0 ; // if set we are inside function parameter list
	int funcbodystart = 0 ; // if set we are inside the function body
	while(ca!=EOF)
	{
				if (ca=='(' || ca ==')' || ca==';' || ca=='=' || ca=='*' || ca=='{' || ca=='}' || ca=='-')
		fprintf(fb,"<%c>",ca);
	
		else if (ca=='\n'){
			fprintf(fb,"\n");
		}
		else if (ca=='\t'||ca==' '){
			
			
		}


		else if(ca == '#'){
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa);
		}
		else if(ca == '"'){
			cb = getc(fa);
			while(cb!='"')
				cb = getc(fa);
		}
		

		else if((ca>='a' && ca<='z') || (ca>='A' && ca<='Z'))// either a keyword or an identifier
		{
			memset(buf,'\0',sizeof(buf) * sizeof(char));
			
	
			

			buf[0]=ca ;int pos = 1;
			cb = getc(fa);
			

			while((cb>='a' && cb<='z') || (cb>='A' && cb<='Z') || (cb>='0' && cb<='9'))
			{
				//check = 1 ;
				buf[pos++]=cb ;
				cb = getc(fa);
				
			}
			fseek(fa, -1, SEEK_CUR);

			buf[pos]='\0';
			for (int j=0;j<11;j++){
				if (strcmp(buf,keyword[j])==0)
				fprintf(fb,"<%s>",buf);}
			for(int i=0;i<tsize;i++)
			{
				
				if(strcmp(buf,arr[i].name)==0)
					fprintf(fb,"<id,%d>",arr[i].index);
			
		
			}
		
			
			
		}
		else{
			fprintf(fb,"<%c>",ca);

		}
		
		ca = getc(fa);
		

	}
	
}

int main()
{
	FILE * fa , *fb ;
	int tsize = 0 ; // no of entries in the symbol table
	struct symbtable arr[50];
	char keyword[11][50]={"int","return","void","char","float","if","else","break","for","do","continue"};
	int size[7]={4,8,8,2,4,1,0};
	int ca , cb ;
	int i ;
	fa = fopen("sample.c","r");
	ca = getc(fa);
	char buf[50];
	char na[3]="NA";
	char keybuf[50]; // will contain the last keyword found
	int ident = 0 ; // this is set to 1 if an identifier has been found
	//int currindex = 0 ; //used to keep track of symbol table index of function which has been found
	int lastkeywordindex = 0 ; // keep track of index of last keyword in the keyword array
	int lastfuncindex = 0 ; // keep track of index of last function
	int insidefuncparam = 0 ; // if set we are inside function parameter list
	int funcbodystart = 0 ; // if set we are inside the function body
	while(ca!=EOF)
	{
		if(ca == '#'){
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa);
		}
		if(ca == '"'){
			cb = getc(fa);
			while(cb!='"')
				cb = getc(fa);
		}
		
		if(ident==1 && ca!=' ' && ca!='\n' &&  ca!='(' && insidefuncparam==0 && funcbodystart==0)
		{
			// global variable found 

			struct symbtable * item = malloc(sizeof(struct symbtable));
			item->index = tsize ;
			tsize ++ ;
			strcpy(item->name,buf);
			strcpy(item->type,keybuf);
			item->size = size[lastkeywordindex];
			item->scope = 'G';
			item->noarg = 0 ;
			item->argument = -1 ;
			strcpy(item->returntype,na);
			arr[item->index] = (*item) ;
			ident=0;
		}



		if((ca>='a' && ca<='z') || (ca>='A' && ca<='Z'))// either a keyword or an identifier
		{
			memset(buf,'\0',sizeof(buf) * sizeof(char));
			int keywordOrIdentifier = 0 ;
	
			

			buf[0]=ca ; int pos = 1;
			cb = getc(fa);
			

			while((cb>='a' && cb<='z') || (cb>='A' && cb<='Z') || (cb>='0' && cb<='9'))
			{
				//check = 1 ;
				buf[pos++]=cb ;
				cb = getc(fa);
				
			}

			buf[pos]='\0';
			for(i = 0 ;i<7;i++)
			{
				if(strcmp(buf,keyword[i])==0)
				{
					keywordOrIdentifier = 1 ;//keyword has been found
					
					break ;
				}
			}


			if(keywordOrIdentifier==0)// keyword not found . Identifier has been found
			{
				
				ident = 1 ;
				if(funcbodystart==0 && insidefuncparam==0)//outside function body and outside function params
				{
				}
				else if((insidefuncparam==0 && funcbodystart==1) || (insidefuncparam==1 && funcbodystart==0) )//a variable either in the body of a function or a parameter of the function
				{
					// local variable has been found

					if(exists(buf, arr, tsize))//dont insert this variable into symbol table if it already exists there
					{
						ident = 0 ;
						ca = getc(fa);
						continue;
					}


					struct symbtable * item = malloc(sizeof(struct symbtable));
					item->index = tsize ;
					tsize ++;
					strcpy(item->name,buf);//name of the variable
					strcpy(item->type,keybuf);//name of the data type that was found just before this variable was found
					item->size = size[lastkeywordindex];
					item->scope = 'L';
					item->noarg = 0 ;
					item->argument = -1 ;
					strcpy(item->returntype,na);





					arr[item->index] = (*item) ;// as arr[tsize-1]


					if(insidefuncparam==1)// if it is a parameter of the function
					{
						arr[lastfuncindex].argument = item->index;
						arr[lastfuncindex].noarg = 1 ;
					}
					ident = 0;
				}
				

			}
			else// keyword has been found
			{
				ident = 0 ;
				lastkeywordindex = i ;
				strcpy(keybuf,buf) ; // now keybuf contains
				// keyword has been found
			}
			
			
				ca = cb;
				continue ;
			

		}
		else if(ca==' ')
		{

		}
		else if(ca=='(' && ident==1)
		{
			// a fn has been found . The value in buffer is the name of the function
			struct symbtable * item = malloc(sizeof(struct symbtable));
			item->index = tsize ;
			tsize ++ ;

			strcpy(item->name,buf);
			char type[]="FUNC";
			strcpy(item->type,type);
			item->scope = 'G' ;
			item->argument = -1 ;
			strcpy(item->returntype,keyword[lastkeywordindex]);
			insidefuncparam = 1 ;
			lastfuncindex = item->index ;

			arr[item->index] = (*item) ;
			ident = 0 ;

		}
		else if(ca==')' && insidefuncparam==1)
		{
			insidefuncparam = 0 ;
		}
		else if(ca=='{')
		{
			funcbodystart = 1 ;
		}
		else if(ca=='}')
		{
			funcbodystart = 0 ;
		}




		ca = getc(fa);

	
	}
	display(arr,tsize-1);
	printf("\n");
	output(arr,tsize-1);
	

}

