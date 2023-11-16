#include<stdio.h>
#include<stdlib.h>

int main(){

    int n;
    printf("Entrer un nombre d'éléments souhaité\n>>> ");
    scanf("%d",&n);
    printf("taille du tableau : %d\n",n);

    int *arr;
    //allocation de la mémoire
    arr=(int *) malloc( n*sizeof(int));

    //initialisation du tableau
    for(int k=0;k<n;k++){
        arr[k]=3*k;
    }


    for(int k=0;k<n;k++){
        printf("%d %d\n",k,arr[k]);
    }

    free(arr);

    return 0;
}
