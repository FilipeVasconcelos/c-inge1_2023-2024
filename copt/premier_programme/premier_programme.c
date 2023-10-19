// Ceci est un commentaire qui commence par //
// notez que le commentaire suivant, est sur 
// plusieurs lignes avec les caractères */  

/*
 Ceci est un premier programme en C.
 Il est composé : 
   i) d'une en-tête <stdio.h> qui permet d'utiliser
      la fonction printf.
  ii) de la fonction main qui est l'entrée principal 
      du programme. C'est cette fonction qui est appelé
      lorsque qu'on execute le fichier produit par la 
      compilation

  Pour compiler :
    $ gcc premier_programme.c

  La compilation produit un fichier a.out (nom par défaut) que 
  l'on execute dans le terminal directement :
    $ ./a.out
*/

// En-tête Standard d'Entrée/Sortie
#include<stdio.h>

// la fonction main retourne un <int>
int main(void) {
    printf("Hellow World!");
    return 0;
}
