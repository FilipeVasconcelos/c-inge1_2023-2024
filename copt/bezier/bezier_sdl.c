#include<stdio.h>
#include<math.h>
#include <stdbool.h>
#include<string.h>
#include <SDL.h>

// Define screen dimensions
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
// some scaling values
#define SCALE_WIDTH    160
#define SCALE_HEIGHT   120
// Some colors
#define WHITE 0xFF,0xFF,0xFF,0xFF
#define BLACK 0x00,0x00,0x00,0xFF
#define POINT_COLOR BLACK 
#define LINE_COLOR 0x00,0xFF,0x00,0xFF

#define MAX_POINTS 10
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

void redefineRectPoints(Point p,SDL_Rect* Rect) {
    Rect->x=p.x;
    Rect->y=p.y;
    Rect->w=10;
    Rect->h=10;
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
                                          SDL_WINDOW_SHOWN);
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
    for (size_t k=0;k<npoints;k++){
        afficher(p[k]);
    }
    SDL_Rect RectPoints[MAX_POINTS];
    for (size_t k=0;k<npoints;k++){
        redefineRectPoints(p[k],&RectPoints[k]);
    }
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
                        p[npoints].x=e.motion.x;
                        p[npoints].y=e.motion.y;
                        for (size_t k=0;k<npoints;k++){
                            redefineRectPoints(p[k],&RectPoints[k]);
                        }
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
        // Set renderer color for the goban  
        SDL_SetRenderDrawColor(renderer, POINT_COLOR);
        // Draw filled goban 
        for (size_t k=0;k<npoints;k++){
            SDL_RenderFillRect(renderer, &RectPoints[k]);
        }
        SDL_SetRenderDrawColor(renderer, LINE_COLOR);
        SDL_Rect line;
        for (float t=0.0;t<=1.0;t=t+0.001){
            Point pout=Plerp ( Plerp( Plerp( Plerp(p[0],p[1],t) , Plerp(p[1],p[2],t) , t ) ,
                                      Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) , t ) , 
                               Plerp( Plerp( Plerp(p[1],p[2],t) , Plerp(p[2],p[3],t) , t ) ,
                                      Plerp( Plerp(p[2],p[3],t) , Plerp(p[3],p[4],t) , t ) , t ), t ) ;
            line.w=5;
            line.h=5;
            line.x=pout.x;
            line.y=pout.y;
            //printf("x : %d y: %d \n",line.x,line.y);
            SDL_RenderFillRect(renderer,&line);
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

