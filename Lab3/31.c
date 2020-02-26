#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct symbtable
{
	int index ; // index of the symbtable
	char name[50];
	char type[4];//type 
	int size ;
	char scope ; //global or local
	int noarg ; // no of arguments
	char returntype[50];
	int argument; // index of the argument of function in the table
};
void display(struct symbtable arr[50] , int size)
{
	//symbol table
	printf("\nIndex\tName\tType\tSize\tScope\tNo of arg\targ index in tab\treturn\n");
	for(int i = 0 ;i<=size;i++)
	{
		printf("%d\t",arr[i].index);
		printf("%s\t",arr[i].name);
		printf("%s\t",arr[i].type);
		printf("%d\t",arr[i].size);
		printf("%c\t",arr[i].scope);
		printf("%d\t",arr[i].noarg);
		if (arr[i].noarg>=1)
			printf("<id,%d>\t\t",arr[i].argument);
		else if(arr[i].noarg==0){
			printf("NULL\t\t");

		}
		else
			printf("\t\t");
		printf("%s\t\t",arr[i].returntype);
		printf("\n\n");

	}
}

int exists(char *buf, struct symbtable arr[50], int tsize)
{// to check if it existsin sumbol table
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
	fa = fopen("test2.c","r");
	fb= fopen("output2.txt", "w");
	ca = getc(fa);
	char buf[50];
	char na[3]=" ";
		char keyword[11][50]={"int","return","void","char","float","if","else","break","for","do","continue"};//keyword
	char keybuf[50]; // will contain the last keyword found
	int ident = 0 ; //set to 1 if an identifier found
	
	int lastkeyind = 0 ; // track of index of last keyword in the keyword array
	int lastfunind = 0 ; // track of index of last function
	int infunpar = 0 ; // set if inside function parameter list
	int funcbodystart = 0 ; // set if inside the function body
	while(ca!=EOF)
	{
		if (ca=='(' || ca ==')' || ca=='>'|| ca=='<'|| ca==';' || ca=='=' || ca=='*' || ca=='{' || ca=='}' || ca=='-')
			fprintf(fb,"<%c>",ca);//certain char
		else if (ca=='\n'){
			fprintf(fb,"\n");
		}
		else if (ca=='\t'||ca==' '){//ignore tab spaces and spaces
				
		}
		else if(ca == '#'){//ignore comments
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa);
		}
		else if(ca == '"'){//ignore double quotes
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
				buf[pos++]=cb ;
				cb = getc(fa);
			}
			fseek(fa, -1, SEEK_CUR);
			buf[pos]='\0';
			for (int j=0;j<11;j++){
				if (strcmp(buf,keyword[j])==0)//check if keyword
				fprintf(fb,"<%s>",buf);}
			for(int i=0;i<tsize;i++)
			{
				//check if present already in symbol table
				if(strcmp(buf,arr[i].name)==0)
					fprintf(fb,"<id,%d>",arr[i].index);
			}
		}
		else if (ca>='0' && cb<='9'){
			memset(buf,'\0',sizeof(buf) * sizeof(char));
			buf[0]=ca;
			cb=getc(fa);int pos=1;
			while (cb>='0' && cb<='9'){
				buf[pos++]=cb;
				cb=getc(fa);
			}
			fseek(fa,-1,SEEK_CUR);
			buf[pos]='\0';
			fprintf(fb,"<num,%s>",buf);
		}
		else{
			//fprintf(fb,"<%c>",ca);
		}
		ca = getc(fa);
	}
}

int main()
{
	FILE * fa , *fb ;
	int tsize = 0 ; // no of entries in the symbol table
	struct symbtable arr[50];
	char keyword[11][50]={"int","return","void","char","printf","if","else","break","for","do","continue"};//keyword
	int size[7]={4,8,8,2,4,1,0};
	int ca , cb ;
	int i ;
	fa = fopen("test2.c","r");
	ca = getc(fa);
	char buf[50];
	char na[3]=" ";
		
	char keybuf[50]; // will contain the last keyword found
	int ident = 0 ; //set to 1 if an identifier found
	
	int lastkeyind = 0 ; // track of index of last keyword in the keyword array
	int lastfunind = 0 ; // track of index of last function
	int infunpar = 0 ; // set if inside function parameter list
	int funcbodystart = 0 ; // set if inside the function body
	while(ca!=EOF)
	{
		if(ca == '#'){//ignore comments
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa);
		}
		if(ca == '"'){//ignore double quotes
			cb = getc(fa);
			while(cb!='"')
				cb = getc(fa);
		}
		if(ident==1 && ca!=' ' && ca!='\n' &&  ca!='(' && infunpar==0 && funcbodystart==0)
		{
			// global variable found 
			struct symbtable * item = malloc(sizeof(struct symbtable));
			item->index = tsize ;
			tsize ++ ;
			strcpy(item->name,buf);
			strcpy(item->type,keybuf);
			item->size = size[lastkeyind];
			item->scope = 'G';
			item->noarg = -1 ;
			item->argument = -1 ;
			strcpy(item->returntype,na);
			arr[item->index] = (*item) ;
			ident=0;
		}
		if((ca>='a' && ca<='z') || (ca>='A' && ca<='Z'))// either a keyword or an identifier
		{
			memset(buf,'\0',sizeof(buf) * sizeof(char));
			int keorId = 0 ;
			buf[0]=ca ; int pos = 1;
			cb = getc(fa);
			while((cb>='a' && cb<='z') || (cb>='A' && cb<='Z') || (cb>='0' && cb<='9'))
			{
				//check = 1 ;
				buf[pos++]=cb ;
				cb = getc(fa);
			}
			buf[pos]='\0';
			for(i = 0 ;i<11;i++)
			{
				if(strcmp(buf,keyword[i])==0)
				{
					keorId = 1 ;//keyword has been found	
					break ;
				}
			}
			if(keorId==0)// keyword not found . Identifier has been found
			{
				ident = 1 ;
				if(funcbodystart==0 && infunpar==0)//outside function body and outside function params
				{
				}
				else if((infunpar==0 && funcbodystart==1) || (infunpar==1 && funcbodystart==0) )//a variable either in the body of a function or a parameter of the function
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
					item->size = size[lastkeyind];
					item->scope = 'L';
					item->noarg = -1 ;
					item->argument = -1 ;
					strcpy(item->returntype,na);
					arr[item->index] = (*item) ;// as arr[tsize-1]
					if(infunpar==1)// if it is a parameter of the function
					{
						arr[lastfunind].argument = item->index;
						arr[lastfunind].noarg = 1 ;
					}
					ident = 0;
				}
			}
			else// keyword has been found
			{
				ident = 0 ;
				lastkeyind = i ;
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
			item->noarg=0;
			item->argument = -1 ;
			strcpy(item->returntype,keyword[lastkeyind]);
			infunpar = 1 ;
			lastfunind = item->index ;
			arr[item->index] = (*item) ;
			ident = 0 ;

		}
		else if(ca==')' && infunpar==1)
		{
			infunpar = 0 ;
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