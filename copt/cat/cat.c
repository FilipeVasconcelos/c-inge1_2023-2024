#include<stdio.h>
int main(){
	int c;
	while ( (c=getchar()) != EOF ) {
	putchar(c); // redirige un caractère vers 
                // la sortie standard (stdout)
	}
	return 0; 
}
