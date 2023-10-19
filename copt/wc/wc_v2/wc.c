#include<stdio.h>
/*
    V2 : 
    Cette version de wc compte le nombre de ligne et de mot dans un
    fichier passé en entrée standard. 
*/
#define IN 1
#define OUT 0

int main(void) {

	int c;
	int nc=0;
	int nl=0;
	int nw=0;
	int wstate=OUT;
	while ((c=getchar()) != EOF) {
		nc++;
		if ( c == '\n' ) {
			nl++;	
		}
		if ( ( c == ' ' ) || ( c == '\t') || ( c == '\n') ) {
			wstate=OUT;
		}
		else if ( wstate == OUT)  {
			wstate=IN;
            nw++;
		}
	}
	printf("%4d %4d %4d\n",nl,nw,nc);
	return 0;	
}
