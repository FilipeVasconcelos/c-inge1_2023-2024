// structure // typedef
// pointeur de structure 
// tableaux de structures

#include<stdio.h>
#include<math.h>

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

double distance(Point p1, Point p2) {
    return sqrt((double) (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}


int main() {
    struct point p1={3,2};
    struct point p2={.x=4,.y=6};
    printf("%d %d\n",p1.x,p1.y);
    struct rect ecran;
    Point P1={.x=0,.y=1};
    Point P2={.x=0,.y=0};
    Point* P3;
    P3=&P1;
    P1.x=0;
    printf("%d %d\n",P3->x,P3->y); // (*P2).x <=> P2->x
    //ecran.p1.x=
    printf("%lf\n",distance(P1,P2));
    
    Point PS[10];
    for (size_t i=0;i<10;i++){
        PS[i].x=0;
        PS[i].y=0;
    }

}
