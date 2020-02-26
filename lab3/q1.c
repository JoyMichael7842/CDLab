#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symtable
{
	int index;
	char name[50];
	char type[10];
	int size;
	char scope;
	int noarg;
	char returntype[50];
	int argument;
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

int exists(char *buf,struct symbtable arr[50],int tsize)
{
	for(int i=0;i<tsize;i++){
		if(strcmp(buf,arr[i].name)==0)
			return 1;
	}
	return 0;
}

void output(struct symbtable arr[50],int tsize){
	FILE *fa,*fb;
	int p=-1;
	int ca,cb;
	int i;
	fa =fopen("sample.c","r");
	fb = fopen("text.txt","w");
	ca = getc(fa);
	char buf[50];
	char na[3] = "NA";
	char keyword[11][50]={"int","return","void","float","if","else","break","for","do","continue"};

	char keybuf[50];
	int ident =0;

	int lastkeywordindex =0;

	int lastfuncindex = 0;
	int insidefuncparam = 0;
	int funcbodystart = 0;

	while(ca!=EOF)
	{
		if(ca == '(' || ca ==')' || ca ==';' || ca =='=' || ca =='*' || ca =='{' || ca =='}' || ca =='-')
		fprintf(fb, "<%c>", ca);

		else if(ca == '\n'){
			fprintf(fb,"\n");
		}
		else if(ca=='\t'||ca==' '){

		}
		else if(ca == '#'){
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa);
		}
		else if((ca>='a' && ca<='z')||(ca>='A' && ca<='Z'))
		{
			
		}
	} 		
}

int main()
{
	FILE *fa,*fb;
	int tsize=0;
	struct symbtable arr[50];
	char keyword[11][50]={"int","return","void","float","if","else","break","for","do","continue"};
	int size[7] = {4,8,8,2,4,1,0};
	int ca,cb,i;
	fa = fopen("sample.c","r");
	ca = getc(fa);
	char buf[50];
	char na[3]="NA";
	char keybuf[50];
	int ident = 0;
	int lastkeywordindex=0;
	int lastfuncindex=0;
	int insidefuncparam = 0;
	int funcbodystart = 0;

	while(ca!=EOF){
		if(ca=="#"){
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa)
		}
		if(ca=='"'){
			cb = getc(fa);
			while(cb!='"')
				cb = getc(fa);
		}
		if(ident==1 && ca!=' ' && ca!='\n' && ca!='(' && insidefuncparam==0 && funcbodystart==0)
		{
			struct symbtable* item = malloc(sizeof(struct symbtable));
			item->index = tsize;
			tsize++;
			strcpy(item->name,buf);
			strcpy(item->type,keybuf);
			item->size = size[lastkeywordindex];
			item->scope = 'G';
			item->noarg = 0;
			item->argument = -1;
			strcpy(item->returntype,na);
			arr[item->index] = (*item);
			ident = 0;
		}

		if((ca>='a' && ca<='z')|| (ca>='A'&&ca<='Z'))
		{
			memset(buf,'\0',sizeof(buf)*sizeof(char));
			int keywordoridentifier=0;

			buf[0]=ca,int pos=1;
			cb=getc(fa);

			while((cb>='a' && cb <='z')||(cb>='A' && cb<='Z')||(cb>='0' && cb<='9'))
			{
				buf[pos++]= cb;
				cb=getc(fa);
			}
			buf[pos] = '\0';
			for(i=0;i<7;i++)
			{
				if(strcmp(buf,keyword[i])==0){
					keywordoridentifier = 1;
					break;
				}
			}

			if(keywordoridentifier==0){
				ident = 1;
				if(funcbodystart == 0 && insidefuncparam == 0)
				{}
				else if((insidefuncparam==0 && funcbodystart==1) || (insidefuncparam==1 && funcbodystart==0))
				{
					if(exists(buf,arr,tsize))
					{
						ident = 0;
						ca=getc(fa);
						continue;
					}

					struct symtable *item = malloc(sizeof(struct symtable));
					item->index = tsize;
					tsize++;
					strcpy(item->name,buf);
					strcpy(item->type,keybuf);
					item->size = size[lastkeywordindex];
					item->scope = 'L';
					item->noarg = 0;
					item->argument = -1;
					strcpy(item->returntype,na);

					arr[item->index] = (*item);

					if(insidefuncparam==1){
						arr[lastfuncindex].argument = item->index;
						arr[lastfuncindex].noarg = 1;
					}
					ident = 0;

				}

			}
			else
			{
				ident = 0;
				lastkeywordindex = i;
				strcpy(keybuf,buf);
			}

			ca = cb;
			continue;
		}
		else if(ca==' ')
		{

		}
		else if(ca == '(' && ident==1)
		{
			struct symbtable *item = malloc(sizeof(struct symtable));
			item->index = tsize;

			tsize++;

			strcpy(item->name,buf);
			char type[] = "FUNC";
			strcpy(item->type,type);
			item->scope = 'G';
			item->argument = -1;
			strcpy(item->returntype,keyword[lastkeywordindex]);
			insidefuncparam = 1;
			lastfuncindex = item->index;

			arr[item->index] = (*item);
			ident = 0;
		}

		else if(ca == ')' && insidefuncparam==1)
		{
			insidefuncparam = 0;
		}
		else if(ca=='{')
		{
			funcbodystart = 1;
		}
		else if(ca == '}')
		{
			funcbodystart = 0;
		}

		ca = getc(fa);

	}	
	display(arr,tsize-1);
	printf("\n");
	output(arr,tsize-1);
}