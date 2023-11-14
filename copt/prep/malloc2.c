#include<stdio.h>
#include<stdlib.h>

// Le programme permet de lire un fichier "data" composé de 3 
// colonnes de données sous la forme suivante:
//
// data :
// n 
// x0 y0 z0
// x1 y1 z1
// ...
// x(n-1) y(n-1) z(n-1)
// 
// où n est le nombre de lignes dans le fichier
//
//
int main()
{
    int nbpoints; // nbre de points à lire dans le fichier
    FILE *fptr; // pointeur sur le fichier
    fptr=fopen("data","r");
    if (!fptr) printf("Erreur de lecture du fichier data");
    fscanf(fptr,"%d",&nbpoints);
    printf("nombre de points (lu): %d\n",nbpoints);
    // déclaration de 3 pointeurs sur les 3 tableaux des données
    // qui vont être lues dans le fichier data 
    double *px,*py,*pz;

    px=malloc( nbpoints *  sizeof(*px) );
    py=malloc( nbpoints *  sizeof(*py) );
    pz=malloc( nbpoints *  sizeof(*pz) );
    // on teste l'allocation de mémoire
    if ( ( px == NULL) || ( py == NULL) || ( pz == NULL) ) 
    {
        printf("Erreur lors de l'allocation de mémoire de p\n");
        exit(1);
    } 

    // on lit (avec fscanf) les données dans le fichier 
    int k=0; 
    while ( fscanf(fptr,"%lf %lf %lf",px+k,py+k,pz+k) != EOF ) 
    {
        k++;
    }
    // on ferme le fichier
    fclose(fptr);
    // on libère la mémoire
    free(pz);
    free(py);
    free(px);
    // programme terminé
    return 0;
}
