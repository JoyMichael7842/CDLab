#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fa,*fb;
	int ca , cb;
	fa=fopen("b.c","r");
	if(fa==NULL){
		printf("no file");
		exit(0);
	}

	fb=fopen("bout.c","w");

	

	while(ca != EOF){
		
		ca=getc(fa);
		if(ca == '#')
			{
				while(ca!='\n')  {
					ca=getc(fa);
				}
				
			}
		if(ca == '"'){
			while(ca!= '\n')
				{
				putc(ca,fb);
				ca=getc(fa);
		} 

	}

			putc(ca,fb); 
	
		

	}

	fclose(fa);
	fclose(fb);
	return 0;
}
