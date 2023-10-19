#include<stdio.h>

int main(void) {

    /* 
       Ecrire un programme qui dit d'un nombre si il est
	   pair ou impair
	*/

	int nombre;
	printf("Entrer un nombre\n>>> ");
	scanf("%d", &nombre);
	printf("%d\n", nombre);
    // Syntaxe du bloc if/else
	/*
		if ( <condition> ) {
			<instructions> si <condition> est vraie (!=0)
		} else {
			<instructions> si <condition> est fausse (==0)
		}
	*/
	if ( nombre % 2 == 0 ) {
		printf("le nombre %d est pair (condition est %d) \n",
                nombre,nombre % 2==0);
	} else {
		printf("le nombre %d est impair (condition est %d)\n",
                nombre,nombre % 2==0);
	}

    // Syntaxe du bloc if/else if/else
	/*
		if (<condition1>) {
			<instructions> si <condition1> est vraie (!=0) 
		}
		else if (<condition2>) {
			<instructions> si <condition2> est vraie (!=0)
		}
		else {
			<instructions> si <condition1> & <condition2> sont fausses 
		}
	*/

	if ( nombre %2 ==0 ) {
		printf("le nombre %d est divisible par deux (condition1 est %d) \n",
                nombre,nombre % 2==0);
	} else if ( nombre %3 ==0 ) {
		printf("le nombre %d est divisible par trois (condition2 est %d) \n",
                nombre,nombre % 3==0);
	} else {
		printf("ni divisible par deux ni par trois\n");
	}	

	return 0;

}

