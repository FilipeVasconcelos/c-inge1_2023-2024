#include<stdio.h>
#include<stdlib.h>

typedef struct {
    float x;
    float y;
    float z;
} ATOM;

void afficher(ATOM* atom){
    printf("%1.2f %1.2f %1.2f\n",atom->x,atom->y,atom->z);
}

int main(){

    ATOM a={.x=0.0,.y=0.0,.z=0.0};
    int n;
    FILE *f;
    f=fopen("data","r");
    fscanf(f,"%d\n",&n);
    printf("taille des données : %d\n",n);

    ATOM* atoms;
    atoms=(ATOM*) calloc(n*sizeof(ATOM));

    for(int k=0;k<n;k++){
        fscanf(f,"%f %f %f",&(atoms[k].x),&(atoms[k].y),&(atoms[k].z));
        afficher(atoms+k);
    }


    fclose(f);
    free(atoms);
    return 0;
}
