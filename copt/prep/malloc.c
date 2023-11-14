#include<stdio.h>
#include<stdlib.h> // librairie qui introduit les fonctions malloc et free

int main()
{
    // on se donne un pointeur NULL sur un int
    int *p=NULL;
    // on alloue de la mémoire pour un int 
    // malloc retourne une adresse que l'on affecte au pointeur
    p=malloc(sizeof(int));
    // on teste si ce pointeur est nul 
    if ( !p ) 
    {
        printf("Erreur lors de l'allocation de mémoire de p\n");
        exit(1);
    } 
    // on change la valeur à l'adresse du pointeur 
    *p=10;    
    printf("adresse : %p valeur: %d\n",p,*p);
    // on libère la mémoire allouée
    free(p);
    return 0;
}
