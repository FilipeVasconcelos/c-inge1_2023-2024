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

#define MAX_POINTS 5 
#define SIZE_POINTS 20
#define HALF_SIZE_POINTS SIZE_POINTS/2
#define LINE_SIZE HALF_SIZE_POINTS
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
  return a + t * (b-a);
}

Point Plerp(Point p1, Point p2, float t) {
    Point p3 = { lerp(p1.x,p2.x,t),
                 lerp(p1.y,p2.y,t)};
    return p3;
}

Point BezierPoint(int n, float t, Point P[], size_t kfirst, size_t klast){
    if ( n == 2 ) {
        return Plerp(P[kfirst], P[klast], t ); 
    }
    else {
        return Plerp(BezierPoint(n-1,t,P, kfirst,kfirst+1),
                     BezierPoint(n-1,t,P, klast-1,klast)  , t ) ;
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


    Point p[MAX_POINTS];

    int npoints=0;
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
                        p[npoints%MAX_POINTS].x=(float)(tmpx);
                        p[npoints%MAX_POINTS].y=(float)(tmpy);
                        redefineRectPoints(p[npoints%MAX_POINTS],&RectPoints[npoints%MAX_POINTS]);
                        for(size_t k=0;k<MIN(npoints,MAX_POINTS);++k){
                            afficher(p[k]);
                        }
                        printf("last pt :%d ",npoints%MAX_POINTS);
                        afficher(p[npoints]);
                        putchar('\n');
                        npoints++;
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
        for (size_t k=0;k<MIN(npoints,MAX_POINTS);++k){
            SDL_RenderFillRect(renderer, &RectPoints[k]);
        }
        SDL_SetRenderDrawColor(renderer, LINE_COLOR);
        SDL_Rect line;
        Point pout,pout2;
        if (npoints > 1 ) {
            for (float t=0.0;t<=1.0;t=t+0.001){
                pout2 = BezierPoint(npoints,t,p,0,npoints);
                if (npoints == 2 ) {
                    pout = Plerp(p[0],p[1],t);
                    printf("Plerp :");
                    afficher(pout);
                    printf("Recursif :");
                    afficher(pout2);
                }
                else if (npoints == 3) {

                    pout = Plerp( Plerp(p[0],p[1],t) ,
                                  Plerp(p[1],p[2],t) , t );
                }
                else if (npoints == 4) {
                    pout = Plerp ( Plerp( Plerp(p[0],p[1],t) ,
                                          Plerp(p[1],p[2],t) , t ) , 
                                   Plerp( Plerp(p[1],p[2],t) ,
                                          Plerp(p[2],p[3],t) , t ) , t ) ;
                }
                else if (npoints == 5) {
                     pout=Plerp ( Plerp( Plerp( Plerp(p[0],p[1],t) , Plerp(p[1],p[2],t) , t ) ,
                                         Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) , t ) , 
                                  Plerp( Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) ,
                                         Plerp( Plerp(p[2],p[3],t) , Plerp(p[3],p[4],t) , t ) , t ), t ) ;
                }
                /*
                else if (npoints >= 6) {
                     pout=
                          Plerp ( Plerp ( Plerp( Plerp( Plerp(p[0],p[1],t) , Plerp(p[1],p[2],t) , t ) ,
                                                 Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) , t ) , 
                                          Plerp( Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) ,
                                                 Plerp( Plerp(p[2],p[3],t) , Plerp(p[3],p[4],t) , t ) , t ), t ) , 
                                  Plerp ( Plerp( Plerp( Plerp(p[0],p[1],t) , Plerp(p[1],p[2],t) , t ) ,
                                                 Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) , t ) , 
                                          Plerp( Plerp( Plerp(p[1],p[2],t) , Plerp(p[3],p[4],t) , t ) ,
                                                 Plerp( Plerp(p[3],p[4],t) , Plerp(p[4],p[5],t) , t ) , t ), t ) , t ) ;
                }
                */
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

