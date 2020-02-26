#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fa,*fb;
	int ca,cb;

	fa = fopen("test.c","r");
	fb = fopen("output.txt","w");


	ca = getc(fa);
	while(ca!=EOF){
		if(ca == '/'){
			cb = getc(fa);
			if(cb == '/'){
				while(ca!='\n'){
					ca = getc(fa);
				}
			}
			else if(cb == '*'){
				do{
					while(ca != '*'){
						ca = getc(fa);	
					}
					ca = getc(fa);
				}while(ca!='/');
				ca = getc(fa);
			}
			else{
				putc(ca,fb);
				putc(cb,fb);
			}
		}
		else{
			putc(ca,fb);
			ca= getc(fa);
		}
	}
	fclose(fa);
	fclose(fb);

	return 0;
}