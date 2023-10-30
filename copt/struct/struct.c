#include<stdio.h>
#include<math.h>
// déclaration d'une structure qui comporte deux <int>
/*
struct point {
    int x;
    int y;
};
*/

// typedef permet de définir un nouveau type. 
// On l'utilise très souvent avec les structures
typedef struct point {
    float x;
    float y;
} Point;

void afficher(Point p){
    printf("x : %f y : %f\n",p.x,p.y);
}
void translater(Point* p, int dx, int dy) {
    p->x += dx; // équivalent à (*p).x += dx; 
    p->y += dy; // équivalent à (*p).y += dy;
}
// Fonction qui retourne un point milieu de deux autres points
Point milieu ( Point p1, Point p2 ) {
    Point p3 = { (p1.x + p2.x) * 0.5 , 
             (p1.y + p2.y) * 0.5 };
    return p3;
}  

double distance(Point p1, Point p2) {
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}


int main(void){
    Point p1={0,0};
    Point p2={1,0};
    printf("distance p1 <-> p2 %lf\n",distance(p1,p2));
    translater(&p1,2,1);
    printf("après la fonction translater\n");
    afficher(p1);
    afficher(p2);
    printf("milieu : \n");
    afficher(milieu(p1,p2));
    return 0;
}

