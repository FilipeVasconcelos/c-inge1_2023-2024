/*
 Les types de données en C

 Les données manipulées en langage C sont typées, c'est-à-dire que 
 pour chaque donnée que l'on utilise (dans les variables par exemple) 
 il faut préciser le type de donnée, ce qui permet de connaître 
 l'occupation mémoire (le nombre d'octets) de la donnée ainsi 
 que sa représentation.

 Les types les plus fréquents :

 +----------------+----------------+-------------------+------------------+
 | Type de donnée | Signification  |  Nombre d'octets  | plage de valeurs |
 +----------------+----------------+-------------------+------------------+
 |    <char>      |    entier      |       1           |    -128 à 127    | 
 +----------------+----------------+-------------------+------------------+
 |    <int>       |    entier      |       4           | -2**31 à 2**31   |
 +----------------+----------------+-------------------+------------------+
 |    <float>     |    flottant    |       4           |    3.4*10-38     |
 |                |                |                   |   à 3.4*10**38   |
 +----------------+----------------+-------------------+------------------+
*/

#include<stdio.h>

int main(){
    //déclarations et affectation de variables 
    int age=40; // base decimal
    float taille=1.83; // convention IEEE
    char initial='F';  // ASCII 
    printf("Mon âge est : (en décimal) %d (en octal) %o (en hexadecimal) %x\n",age,age,age);
    printf("Ma taille est : %f %25.17f %12.2f\n",taille,taille,taille);
    printf("Mon initial est : (ASCII) %c (decimal) %d (hexadecimal) %x\n",initial,initial,initial);

    printf("caractères imprimables:\n");
    for(int i=32;i<127;i++){
        printf("%c",i);
    }
    putchar('\n');

    return 0;
}
