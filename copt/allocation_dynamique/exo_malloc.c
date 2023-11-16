/* 
   Ecrire un programme qui permet de lire un fichier 
   "data" composé de 3 colonnes de données 
   sous la forme suivante:

n 
x0 y0 z0
x1 y1 z1
...
x(n-1) y(n-1) z(n-1)
 
où n est le nombre de lignes dans le fichier
*/
#include<stdio.h>
#include<stdlib.h>

int main(){

    int n;
    FILE *f;
    f=fopen("data","r");
    fscanf(f,"%d\n",&n);
    printf("taille des données : %d\n",n);

    float *X,*Y,*Z;
    X=(float*) malloc(n*sizeof(float));
    Y=(float*) malloc(n*sizeof(float));
    Z=(float*) malloc(n*sizeof(float));
    if ((!X) || (!Y) || (!Z) ){
        printf("Erreur !! Il va falloir acheter la RAM");
        exit(1);
    }
    for(int k=0;k<n;k++){
        fscanf(f,"%f %f %f",X+k,Y+k,Z+k);
        printf("X : %f Y : %f Z : %f\n",X[k],Y[k],Z[k]);
    } 

    fclose(f);
    free(X);
    free(Y);
    free(Z);

    return 0;
}
