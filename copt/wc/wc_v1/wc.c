#include<stdio.h>
/*
  Première version de la commande wc.
  Le programme lis l'entrée standard et compte le nombre total de 
  caractères.
*/
int main(void) 
{
    int c;
    int nc=0;
	while ((c=getchar()) != EOF) {
		nc++;
	}
	printf("%4d\n",nc);
	return 0;	
}
