/*
  Écrivez une fonction qui prend un pointeur sur un tableau d'entiers 
  et qui incrémente tous les éléments du tableau de 1.
*/
#include<stdio.h>

#define N 4

void increment(int n, int* arr) {
	for(int i=0;i<n;i++){
		arr[i]++;
	}	
}

int main(void) {

	int arr[4]={0,1,2,3};
	for(int i=0;i<N;i++) { 
		printf("%d %d\n",i,arr[i]);
	}
	putchar('\n');
	increment(N,arr);
	for(int i=0;i<N;i++) { 
		printf("%d %d\n",i,arr[i]);
	}
	
	return 0;
}
