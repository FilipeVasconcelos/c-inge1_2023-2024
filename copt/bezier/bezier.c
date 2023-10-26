#include<stdio.h>
#include<math.h>
#include<string.h>
#define MAX_POINTS 10
typedef struct point {
    float x;
    float y;
} Point;

void afficher(Point p){
    printf("x : %f y : %f\n",p.x,p.y);
}
void ecrirePoint(FILE * stream,Point p){
    fprintf(stream,"%f %f\n",p.x,p.y);
}

float lerp(float a, float b, float t) {
  return a + t * (b-a);
}

Point Plerp(Point p1, Point p2, float t) {
    Point p3 = { lerp(p1.x,p2.x,t),
                 lerp(p1.y,p2.y,t)};
    return p3;
}


int main(void){
    Point p[MAX_POINTS];
    float xx;
    float yy;
    FILE *fptr;
    fptr = fopen("data", "r");
    int k=0;
    while ( fscanf(fptr, "%f %f\n", &xx, &yy)!=EOF ) {
        p[k].x=xx;
        p[k].y=yy;
        k++;
    }
    fclose(fptr);
    int npoints=k;
    for (size_t k=0;k<npoints;k++){
        afficher(p[k]);
    }

    FILE *out;
    out = fopen("bezier.log", "w");
    for (float t=0.0;t<=1.0;t=t+0.01){
        ecrirePoint( out, Plerp ( Plerp( Plerp( Plerp(p[0],p[1],t) , Plerp(p[1],p[2],t) , t ) ,
                                         Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) , t ) , 
                                  Plerp( Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) ,
                                         Plerp( Plerp(p[2],p[3],t) , Plerp(p[3],p[4],t) , t ) , t ), t ) ) ;
    }
    fclose(out);

    return 0;
}

