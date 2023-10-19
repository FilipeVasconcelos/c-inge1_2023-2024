#include<stdio.h>

// les prototypes des fonctions f et g se trouvent dans les fichiers "header"
// des variables globales peuvent également être placées dans ces fichiers
#include"f.h"
#include"g.h"

//Il est toujours possible de ne donner que les prototypes dans les fichiers qui appellent
//les fonctions en question
/*
int f(int x);
int g(int x);
*/

int main()
{
    for (int x=0;x<5;x++)
    {
        printf("x : %d f(x) : %3d g(x) : %6.3f\n",x,f(x),g((float)x) );
    }
}
