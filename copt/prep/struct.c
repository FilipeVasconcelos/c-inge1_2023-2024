// structure // typedef
// pointeur de structure 
// tableaux de structures

#include<stdio.h>

struct point{
    int x;
    int y;
};

typedef struct Point{
    int x;
    int y;
} Point;

struct rect{
    struct point p1;
    struct point p2;
};

int main() {
    struct point p1={3,2};
    struct point p2={.x=4,.y=6};
    printf("%d %d\n",p1.x,p1.y);
    struct rect ecran;
    Point P1={.x=6,.y=2};
    Point* P2;
    P2=&P1;
    P1.x=0;
    printf("%d %d\n",P2->x,P2->y); // (*P2).x <=> P2->x
    //ecran.p1.x=
}
