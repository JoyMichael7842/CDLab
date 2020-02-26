


int main(){
	FILE *fa,*fb;
	int ca,cb;
	fa = fopen("b.c","r");
	if(fa == NULL){
		printf("cannot open file \n");
		exit(0);
	}
	fb = fopen("bout.c","w");
	ca = getc(fa);
	while(ca!= EOF){
		if(ca == ' '){
			cb = getc(fa);
			if(cb == ' '){
				while(ca!=' ')
					ca = getc(fa);
			}
			else{
				putc(ca,fb);
			}
		}
		else{
			putc(ca,fb);
		}
		ca = getc(fa);  
	}
	fclose(fa);
	fclose(fb);
	return 0;
}
ÿ