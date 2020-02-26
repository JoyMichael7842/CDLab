#include <stdio.h>
#include <stdlib.h>

int main(){		
	FILE *fa,*fb;
	int ca,cb,cx;
	fa=fopen("a.c","r");
	if(fa==NULL){
		printf("cannot open file");
		exit(0);
	}

	fb=fopen("aout.c","w");
	



	while(ca != EOF) {
	
		ca=getc(fa);
		cb=getc(fa);
		if(ca != ' ' && ca !='\t' && ca!= '\n' ) {
			putc(ca,fb);
		}
			
		else {
			while(cb!= ' ' && cb != '\t' && cb !='\n')
			cb=getc(fa)
			putc(' ',fb);
		}

	}

	fclose(fa);
	fclose(fb);

	
}
