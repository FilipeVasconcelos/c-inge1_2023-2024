#include<stdio.h>


int main(){
	/* 
	    // Syntaxe de la boucle for
		for (<règles d'iterations>) {
			<instructions répétées> 
		}
	*/
	/*
		Afficher tous les chiffres de 0 à 9 
	*/
	for ( int i=0; i < 10 ; i++) {
		printf("%d\n",i);
	}
	/*
		Afficher tous les chiffres de 0 à 9 de 
		deux en deux (en commençant 0)
	*/
	putchar('\n');
	for ( int i=0; i < 10 ; i=i+2) {
		printf("%d\n",i);
	}
	/*
		while (<condition>) {
			<instructions répétées> tant que
			<condition> != 0
		}
	*/
	putchar('\n'); /// il existe un équivalent en entrée getchar();
	int i=0;
	while ( i<10 ) {
		printf("%d\n",i);
		i++;
	}
}
