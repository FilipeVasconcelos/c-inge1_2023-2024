#include<stdio.h>
#include<stdlib.h>

int main(){

    // void* malloc ( nombre d'octets )
    //sizeof() retourne la taille en octet d'un objet 
    int* ptr;
    // ptr est un pointeur sur un int
    ptr=(int*) malloc(sizeof(int));
    *ptr = 6;

    printf("%d\n",*ptr);
    free(ptr);
    return 0;
}
