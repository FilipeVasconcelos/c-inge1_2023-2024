#include<stdio.h>
#include<stdlib.h> // librairie qui introduit les fonctions malloc et free

typedef struct Point{
    float x;
    float y;
} Point;

void afficher(Point* pt){
    printf("%f %f\n",pt->x,pt->y);
}

int main(int argc, char *argv[])
{
    if (argc<2) {
        printf("Usage : %s <file> \n",*argv);
        printf("<file> est un fichier texte\n");
        exit(1);
    } 
    FILE *fp = fopen( argv[ 1 ], "r" );
    // n sera la taille du tableau
    int n;
    fscanf(fp, "%d", &n);
    // on se donne un pointeur qui correspondra à notre tableau.
    Point* Points=(Point*) malloc(4*n*n*sizeof(Point));
    // on teste si ce pointeur est nul 
    if ( !Points ) 
    {
        printf("Erreur lors de l'allocation de mémoire de p\n");
        exit(1);
    } 
    int k=0;
    for (int i = 0; i < n; i++ ) {
        for (int j = 0; j < n; j++ ) {
            Points[k].x=i;
            Points[k].y=j;
            k++;
            Points[k].x=i+0.333;
            Points[k].y=j+0.666;
            k++;
            Points[k].x=i+0.666;
            Points[k].y=j+0.333;
            k++;
            Points[k].x=i+0.333;
            Points[k].y=j+0.333;
            k++;
        }
    }


    for (int k = 0; k < 4*n*n; k++ ) {
        afficher(Points+k);
    }


    free(Points);
    return 0;
}
