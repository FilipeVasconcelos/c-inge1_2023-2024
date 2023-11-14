#include<stdio.h>
#include<stdlib.h> // librairie qui introduit les fonctions malloc et free

int main()
{
    // n sera la taille du tableau
    int n;
    // on se donne un pointeur qui correspondra à notre tableau.
    int* arr=NULL;
    printf("Entrez la taille du tableau : ");
    scanf("%d", &n);

    arr=(int*) malloc(n* sizeof(int));
    // on teste si ce pointeur est nul 
    if ( !arr ) 
    {
        printf("Erreur lors de l'allocation de mémoire de p\n");
        exit(1);
    } 
    // Initialisation du tableau
    for (int i = 0; i < n; i++) {
        arr[i] = i * 2;
    }
    // Affichage du tableau
    printf("Contenu du tableau : ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
    return 0;
}
