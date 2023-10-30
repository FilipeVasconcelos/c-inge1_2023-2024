#include<stdio.h>
#include<math.h>
#include <stdbool.h>
#include<string.h>
#include <SDL.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
// Define screen dimensions
#define SCREEN_WIDTH    1200 
#define SCREEN_HEIGHT   800
// Some colors
#define WHITE 0xFF,0xFF,0xFF,0xFF
#define BLACK 0x00,0x00,0x00,0xFF
#define POINT_COLOR 0xCC,0x25,0x29,0xFF 
#define LINE_COLOR 0x39,0x6A,0xB1,0xFF

#define MAX_POINTS 4 
#define SIZE_POINTS 20
#define HALF_SIZE_POINTS SIZE_POINTS/2
#define LINE_SIZE HALF_SIZE_POINTS/2
#define NLINEPOINTS 512 
typedef struct point {
    float x;
    float y;
} Point;

double distance(Point p1, Point p2) {
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}


void afficher(Point p){
    printf("x : %f y : %f\n",p.x,p.y);
}
void ecrirePoint(FILE * stream,Point p){
    fprintf(stream,"%f %f\n",p.x,p.y);
}

float lerp(float a, float b, float t) {
  return (1 - t) * a + t * b;
}

Point Plerp(Point p1, Point p2, float t) {
    Point p3 = { lerp(p1.x,p2.x,t),
                 lerp(p1.y,p2.y,t)};
    return p3;
}

/*
 Cette permet de donner le point de la courbe de Bézier pour une valeur
 du paramètre t donnée pour un nombre de points n quelconques donnée dans
 un tableau P.

 n : nombre de points
 f : first index
 l : last index
 P : tableaux de Point 
 t : paramètre de la fonction paramétrique


  Bezier recursif :
  n=2 interval indice 0,1
    Plerp(0,1,t)
  n=3 interval indice 0,2 
    Plerp( Bezier(2,0,1,p,t) , Bezier(2,1,2,p,t) , t ) 
  n=4 interval indice 0,3
    Plerp( Bezier(3,0,2,p,t) , Bezier(3,1,3,p,t) ,t ) 
  n=5 interval indice 0,4
    Plerp( Bezier(4,0,3,p,t) , Bezier(3,1,4,p,t) ,t ) 
  n interval indice f,l
    Plerp(Bezier(n-1,f,f+n-2), Bezier(n-1,f+1, l ) , t ) 
*/
Point BezierPoint(int n, size_t f, size_t l, Point P[], float t){
    if ( n == 2 ) {
        return Plerp(P[f], P[l], t ); 
    }
    else {
        return Plerp(BezierPoint(n-1,f,f+n-2,P,t),
                     BezierPoint(n-1,f+1,l,P,t)  , t ) ;
    }
}

void redefineRectPoints(Point p,SDL_Rect* Rect) {
    Rect->x=p.x;
    Rect->y=p.y;
    Rect->w=SIZE_POINTS;
    Rect->h=SIZE_POINTS;
}
int main(void){

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr,"SDL failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    // Create window
    SDL_Window *window = SDL_CreateWindow("Exemple SDL2 Parcours Innovation",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if(!window)
    {
       fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
       return 1;
    }
    
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        fprintf(stderr,"SDL_Create_Rendeder failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    float t;
    float dt=1.0/NLINEPOINTS;
    int pindex=0; // point index
    Point p[MAX_POINTS];
    SDL_Rect RectPoints[MAX_POINTS];
    bool quit = false;
    SDL_Event e;
    while(!quit)
    {
        SDL_PollEvent(&e);
        // check for messages
        switch (e.type) {
            case SDL_MOUSEBUTTONDOWN:
                 switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        int tmpx,tmpy;
                        SDL_GetMouseState(&tmpx, &tmpy);
                        p[pindex%MAX_POINTS].x=(float)(tmpx);
                        p[pindex%MAX_POINTS].y=(float)(tmpy);
                        redefineRectPoints(p[pindex%MAX_POINTS],&RectPoints[pindex%MAX_POINTS]);
                        printf("--------------\n");
                        printf("npoints : %d\n",MIN(pindex+1,MAX_POINTS));
                        printf("--------------\n");
                        for(size_t k=0;k<MIN(pindex+1,MAX_POINTS);++k){
                            afficher(p[k]);
                        }
                        putchar('\n');
                        pindex++;
                        break;
                  }
                break;
            case SDL_QUIT :
                quit = 1; 
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                        quit = 1;
                        break;
                    }
            default:
                break;
        }
        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, WHITE);
        // Clear screen
        SDL_RenderClear(renderer);
        // Set renderer color Point
        SDL_SetRenderDrawColor(renderer, POINT_COLOR);
        // Draw filled rect Points
        for (size_t k=0;k<MIN(pindex,MAX_POINTS);++k){
            SDL_RenderFillRect(renderer, &RectPoints[k]);
        }
        SDL_SetRenderDrawColor(renderer, LINE_COLOR);
        SDL_Rect line;
        Point pout,pout2;
        if ( MIN(pindex,MAX_POINTS) > 1  ) {
            for (size_t kt=0;kt<NLINEPOINTS;kt++){
                t = (float)kt*dt;
                pout = BezierPoint(MIN(pindex,MAX_POINTS),0,MIN(pindex-1,MAX_POINTS-1),p,t);
                line.w=LINE_SIZE;
                line.h=LINE_SIZE;
                line.x=pout.x+HALF_SIZE_POINTS;
                line.y=pout.y+HALF_SIZE_POINTS;
                //printf("x : %d y: %d \n",line.x,line.y);
                SDL_RenderFillRect(renderer,&line);
                }
        }
        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);
    // Quit SDL
    SDL_Quit();

    return 0;
}

