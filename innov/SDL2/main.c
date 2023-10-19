#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Library SDL
// sudo apt install build-essential pkg-config libsdl2-dev
#include <SDL.h>

// LIbrary SDL_TTF
// sudo apt install libsdl2-ttf-dev libsdl-ttf2.0-dev
#include "SDL_ttf.h"

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

// linear interpolation
int lerp(int a,int b, float t){
    return a+(b-a)*t;
}
// linear interpolation between two colors
SDL_Color get_lerp(SDL_Color c1, SDL_Color c2, float t){
    SDL_Color r={lerp(c1.r,c2.r,t),
            lerp(c1.g,c2.g,t),
            lerp(c1.b,c2.b,t)};
    return r;
}

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr,"SDL failed to initialise: %s\n",SDL_GetError());
        return 1;
    }

    if (TTF_Init()<0) 
    {
        fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
        SDL_Quit();
        return 2;
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
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        fprintf(stderr,"SDL_Create_Rendeder failed to initialise: %s\n",SDL_GetError());
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    //this opens a font style and sets a size
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 18);
    if(!font) 
    {
        fprintf(stderr,"TTF_OpenFont failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    SDL_Color textColor = {0,0,0};
    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    SDL_Surface* surfaceMessage[2] = { TTF_RenderText_Solid(font, "SPACE : change color",textColor),
                                       TTF_RenderText_Solid(font, "UP/LEFT/DOWN/RIGHT : move",textColor) } ;

    // now you can convert it into a texture
    SDL_Texture* Message[2];
    for (int i =0; i<2;i++) {
        Message[i]=SDL_CreateTextureFromSurface(renderer, surfaceMessage[i]);
    }
        
    // Declare a square as Rect 
    SDL_Rect square;
    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    square.w = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    square.h = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    // Square position: In the middle of the screen
    square.x = SCREEN_WIDTH / 2 - square.w / 2;
    square.y = SCREEN_HEIGHT / 2 - square.h / 2;

    // Position, width, weight of the Message Rect
    int texW[2] = {0,0};
    int texH[2] = {0,0};
    for (int i =0; i<2;i++) {
        SDL_QueryTexture(Message[i], NULL, NULL, &texW[i], &texH[i]);
    }
    SDL_Rect MessageRect[2]={ {square.x,square.y,texW[0],texH[0]},
                              {square.x,square.y+texH[0],texW[1],texH[1]}
                            };

    srand( time( NULL ) );
    // la couleur c[2] est une interpolation linéaire de c[0] et c[1] 
    SDL_Color c[3]={        
        {.r = 255,0,0}, //rouge
        {.r = 0,0,255}, //bleu
        {.r = 0 , .g = 0, .b = 0}
    };
    c[2].r=c[0].r;
    c[2].g=c[0].g;
    c[2].b=c[0].b;
    int r,alpha;
    alpha=255;
    float tlerp=0.0;
    float clerp=1.0;

    printf("<Space> to move in color space\n"  
           "<UP>/<LEFT>/<DOWN>/<RIGHT> to move the square\n"
           "<q> to quit\n");
    // Event loop
    bool quit = false;
    SDL_Event e;
    while(!quit)
    {

        SDL_PollEvent(&e);

        // check for messages
        switch (e.type) {
            case SDL_QUIT :
                quit = 1; 
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_SPACE:
                        r=rand();
                        if ( (tlerp>=1.) || (tlerp<0.0) ){
                            clerp*=-1.0;
                        }
                        tlerp+=(1.0/255.0)*clerp;
                        c[2]=get_lerp(c[0], c[1], tlerp);
                        break; 
                    case SDLK_LEFT:
                        square.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        square.x += 5;
                        break;
                    case SDLK_UP:
                        square.y -= 5;
                        break;
                    case SDLK_DOWN:
                        square.y += 5;
                        break;
                    case SDLK_q:
                        quit = 1;
                        break;
                    case SDLK_a:
                        alpha += 1;
                        alpha%=255;
                        break;
                    }
            	default:
                	break;
        }

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // Clear screen
        SDL_RenderClear(renderer);
        // Set renderer color for the square  
        SDL_SetRenderDrawColor(renderer, c[2].r , c[2].g, c[2].b, alpha );
        // Draw filled square
        SDL_RenderFillRect(renderer, &square);

        SDL_Rect MessageRect[2]={ {square.x,square.y,texW[0],texH[0]},
                                  {square.x,square.y+texH[0],texW[1],texH[1]}
                                };
        for (int i=0; i<2;i++) {
           SDL_RenderCopy(renderer, Message[i], NULL, &MessageRect[i]);
        }
        // Update screen
        SDL_RenderPresent(renderer);
    }
    //
    for (int i=0; i<2;i++) {
        SDL_DestroyTexture(Message[i]);
        SDL_FreeSurface(surfaceMessage[i]);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return 0;
}
