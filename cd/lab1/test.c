
#include <stdio.h>
void main()
{
	FILE *fopen(),*fp;
	int c;
	fp = fopen("file.c");

	c= getc(fp);

	while(c !=  EOF)
	{
		putchar(c);
		c = getc(fp);

	}
	
	fclose(fp);

}