#include<stdio.h>
/*
    Version 3 : 
        Permet de lire les options (-l -w -c) et le nom du
        fichier en entrée standard.
    
    Compilation :
        make
    Usage :     
        ./wc [-l -c -w] <fichier>   
*/

#define ALL_MODE 0
#define CHARACTER_MODE 1
#define LINE_MODE 2
#define WORD_MODE 3
#define IN 1
#define OUT 0
#define MAX_LEN 100

int main (int argc, char* argv[])
{
    int c; 
    char** ptr=argv; // pointeur sur argv 
    FILE* file;      // pointeur vers le fichier
    int mode=ALL_MODE; // default mode
    long int nc=0;   // nombre de caractères comptés
    long int nl=0;   // nombre de lignes comptés
    long int nw=0;   // nombre de mots comptés
    int wstate=OUT;  // etat dans l'algo pour compter les mots 
    
    // boucle principale pour lire les arguments/options
    while(*(++ptr)) {

#if debug
    printf("lecture de l'argument : %s\n",*ptr);
#endif
        // si le premier caractère est un '-' -> c'est une option
        if (  **ptr == '-')
        {
            // switch 
            // on teste le deuxième caractère 
            switch ( *((*ptr)+1) ) 
            {
                case 'l':
                    mode=LINE_MODE;
                    break;
                case 'c':
                    mode=CHARACTER_MODE;
                    break;
                case 'w':
                    mode=WORD_MODE;
                    break;
                default:
                    printf("Erreur : option non valide");
                    return 1;
                    break;
            }  
        }
    }
    // fin de la lecture des options
    
    //lecture du nom du fichier dernier argument
	file=fopen(argv[argc-1], "r");

    // tant que la lecture du fichier n'est pas EOF
    while ( (c=fgetc(file)) != EOF) 
    {
        ++nc;
        if (c=='\n') ++nl;
        if (c==' ' || c=='\n' || c=='\t') 
        {
            wstate=OUT;
        }
        else if (wstate==OUT)
        {
            wstate=IN;
            ++nw;
        }
    }
    // Affichage des résultats 
    switch (mode) {
        case LINE_MODE:
            printf("%-ld",nl);
            break;
        case CHARACTER_MODE:
            printf("%-ld",nc);
            break;
        case WORD_MODE:
            printf("%-ld",nw);
            break;
        case ALL_MODE:
            printf("%4ld %4ld %4ld",nl,nw,nc);
            break;
    }
    if ( file ) {
        printf(" %-s\n",argv[argc-1]);
        fclose(file);
    }
    else {
        putchar('\n');
    }
	return 0;	
}
