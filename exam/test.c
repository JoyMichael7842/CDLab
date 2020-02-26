#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	FILE *fa,*fb;
	int ca,cb;

	fa =fopen("in.c","r");

	ca = getc(fa);
	char buf1[50];
	char buf2[50];
	char buf[50];
	int pos = 0;

	while(ca!=EOF){
		if(ca=='#'){
		while(ca!='\n'){
			ca= getc(fa);
		}
	}
		else if(ca=='+'||ca == '-' ||ca == '+'){
			buf[0] = ca;
			buf[1] = '\0';
			printf("<%s,arithmetic operator>",buf);
		}

		else if(ca == ' '|| ca == '\t'){

		}

		else if(ca == '\n'){
			printf("\n");
		}

		else if(ca>='a' && ca<-'z'){
			cb = getc(fa);
			while(cb>='a' && cb <='z'){
				buf1[pos++] = cb;
				cb = getc(fa);
			}
			buf1[pos] = '\0';
			//printf();
		}

		else if(ca >= '0' && ca <= '1'){
			cb = getc(fa);
			while(cb>='0' && cb<='1'){
				buf2[pos++] = cb;
			}
		}


	
		else{
			buf[0] = ca;
			buf[1] = '\0';
			printf("<%s,special symbol>",buf);
			ca = getc(fa);
		}
		ca = getc(fa);

		
	}


}