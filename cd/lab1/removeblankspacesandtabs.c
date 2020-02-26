#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fa,*fb;

	fa = fopen("test.c","r");
	fb  = fopen("output.txt","w");
	int ca,cb;

	ca = getc(fa);
	
	while(ca!=EOF){
		if(ca == ' '){
			cb = getc(fa);
			if(cb == ' '){
				do{
					ca = getc(fa);
				}while(ca==' ');
			}
			putc(ca,fb);
		}
		else if(ca=='\n'||ca=='\t'){
			putc(' ',fb);
			ca = getc(fa);
		}
		else{
			putc(ca,fb);
			ca = getc(fa);
		}
	}
}