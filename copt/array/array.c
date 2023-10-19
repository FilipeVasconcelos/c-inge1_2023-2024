/* Au programme :
 *  Tableaux et pointeurs.
*/
#include<stdio.h>
// En C un tableau est structure de données statique.

// On définira la taille d'un tableau statique à l'aide
// d'une macro 
#define N 10 

// le tableau est passé en référence 
//(puisque arr[10] c'est un pointeur)
void double_valeurs(int n , int* arr){
	for (int i=0;i<n;i++){
		*(arr+i)=*(arr+i)*2;
	}
}
int main(void) {

	int arr[N]={1,2,3,4,5,6,7,8,9,10};
    // parcourir un tableau par les indices
	for (int i=0;i<N;i++) {
		printf("indice %d element : %d\n",i,arr[i]);
	}

	//arr est un pointeur sur le premier élément du tableau
    //il peut être passé par référence dans une fonction
	double_valeurs(N,arr);
    //nouvelle affichage après l'appel de la fonction
	for (int i=0;i<N;i++) {
		printf("indice %d element : %d\n",i,arr[i]);
	}

    //Puisque arr est un pointeur *(arr) représente la valeur à l'adresse
    //de arr. Ainsi arr[i] est équivalent à *(arr+i) 
	for (int i=0;i<N;i++){
		printf("adresse : %p valeur à l'adrese : %d \n",arr+i, *(arr+i) );
	}
	return 0;
}
