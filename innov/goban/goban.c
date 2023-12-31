/* TODO :
 *   - Lire un SGF quelconque
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "draw_circle.h"
#include "tools.h"
#include "board.h"
#include "stone.h"
#include "sgf.h"
#include "colors.h"

#define MIN_SCREEN_W  800
#define MIN_SCREEN_H  600
int screen_width=MIN_SCREEN_W;
int screen_height=MIN_SCREEN_H;
int max_screen_width;
int max_screen_height;
int min_screen_width=MIN_SCREEN_W;
int min_screen_height=MIN_SCREEN_H;

/*
typedef struct Groups{
    size_t liberty;
    Vec2D liberties[NMAX_SEQUENCE];
} Groups;
*/
int main(int argc, char* argv[])
{
    bool readingfromfile=false;
    bool fullscreen=false;
    char sgfilename[MAX_LEN];
    if (argc > 2 ) {
        printf("Usage: %s <SGFfile>\n",*argv);
        exit(1);
    } else if (argc > 1) {
        printf("reading sgf file %s\n",*(argv+1));
        readingfromfile=true;
        strcpy(sgfilename,*(argv+1));
    }
    SDL_Color black;
    set_color(&black,BLACK);
    Board board;
    set_board(&board,screen_width,screen_height);

    Stone stones[NMAX_SEQUENCE];
    size_t played=0;
    size_t showed=0;
    size_t prev_showed=0;

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
    SDL_Window *window = SDL_CreateWindow("CBoard v0.2",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width,screen_height,
                                          SDL_WINDOW_RESIZABLE);
    if(!window)
    {
       fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
       return 1;
    }
    SDL_SetWindowMinimumSize(window, MIN_SCREEN_W, MIN_SCREEN_H);
	// setting minimum size might move the window, so set position again
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    // Create renderer
    //SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(!renderer)
    {
        fprintf(stderr,"SDL_Create_Rendeder failed to initialise: %s\n",SDL_GetError());
        return 1;
    }

    TTF_Font* font16 = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSansBold.ttf", 16);
    TTF_Font* font24 = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSansBold.ttf", 24);
    if(!font16) 
    {
        fprintf(stderr,"TTF_OpenFont failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    if(!font24) 
    {
        fprintf(stderr,"TTF_OpenFont failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    char row[26] = "ABCDEFGHJKLMNOPQRSTUVWXYZ"; 
    SDL_Surface* Surf_row[N];
    SDL_Surface* Surf_col[N];
    char *ptr=malloc(2*sizeof(char));
    char str[3];
    for (size_t i=0;i<N;i++) {
        ptr[0]=row[i];
        ptr[1]='\0';
        Surf_row[i] = TTF_RenderText_Solid(font16, ptr,black) ;
        sprintf(str, "%ld", i+1);
        Surf_col[i] = TTF_RenderText_Solid(font16, str,black) ;
    }
    // now you can convert it into a texture
    SDL_Texture* Text_row[N];
    SDL_Texture* Text_col[N];
    for (size_t i =0; i<N;i++) {
        Text_row[i]=SDL_CreateTextureFromSurface(renderer, Surf_row[i]);
        Text_col[i]=SDL_CreateTextureFromSurface(renderer, Surf_col[i]);
    }

    SDL_Surface* Surf_move=TTF_RenderText_Solid(font24, "Move :", black);
    SDL_Texture* Text_move=SDL_CreateTextureFromSurface(renderer, Surf_move);
 
    SDL_Rect img={0, 0, 0, 0}; // pos and size of both images
    SDL_Surface* wpng = IMG_Load("img/white.png");
    SDL_Surface* bpng = IMG_Load("img/black.png");
    if ((!wpng)||(!bpng))
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }

    SDL_Texture* wstone = SDL_CreateTextureFromSurface(renderer,wpng); 
    SDL_Texture* bstone = SDL_CreateTextureFromSurface(renderer,bpng); 
    SDL_QueryTexture(wstone, NULL, NULL, &img.w, &img.h);
    // background
    draw_background(renderer,&board,Text_row,Text_col,Text_move);
    if (readingfromfile) read_sgfile(sgfilename,board,stones,&played);

    bool quit = false;
    Vec2D line;
    SDL_Event e;
    SDL_Rect textRect;    
    bool showed_to_be_rendered = true;
    // boucle principale des évenements SDL
    while(!quit)
    {
        if (SDL_WaitEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEWHEEL:
                    if (e.wheel.y>0) {
                        showed=(showed+5<=played) ? showed+5: played;
                    } else if (e.wheel.y<0) {
                        showed=((int) (showed-5)>=0) ? showed-5: 0;
                        draw_background(renderer,&board,Text_row,Text_col,Text_move);
                        render_stones(renderer, stones ,img,wstone, bstone,showed);
                    }
                break;
                case SDL_MOUSEBUTTONDOWN:
                     switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            if (readingfromfile) {
                                showed=(showed+1<=played) ? showed+1: played;
                                break;
                            }
                            line.x=get_line(e.motion.x,'x',board);
                            line.y=get_line(e.motion.y,'y',board);
                            if ( (line.x>=0)                && 
                                 (line.y>=0)                && 
                                 (line.x<board.size)        && 
                                 (line.y<board.size)        && 
                                 (linefree(played,stones,line.x,line.y)) ) {
                                set_stone(stones+showed,line.x,line.y,board,played%2);
                                played++;
                                showed++;
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            showed=((int) (showed-1)>=0) ? showed-1: 0;
                            draw_background(renderer,&board,Text_row,Text_col,Text_move);
                            render_stones(renderer, stones ,img,wstone, bstone,showed);
                            break;
                      }
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window,&screen_width,&screen_height);
                        if (screen_width<MIN_SCREEN_W) screen_width=MIN_SCREEN_W;
                        if (screen_height<MIN_SCREEN_H) screen_height=MIN_SCREEN_H;
                        SDL_SetWindowSize(window,screen_width,screen_height);
                        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                        set_board(&board,screen_width,screen_height);
                        draw_background(renderer,&board,Text_row,Text_col,Text_move);
                        // Text move has to be rendered if WINDOWEVENT_RESIZED but only
                        if (prev_showed == showed) { 
                            render_text_from_int(renderer,font24, black, showed,screen_width*0.15,screen_height*0.1);
                        }
                        //reset position in pixel from new board size
                        for (size_t i=0;i<played;++i){
                            set_stone(stones+i,stones[i].linex,stones[i].liney,board,-1);
                        }
                        render_stones(renderer, stones ,img,wstone, bstone,showed);
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_f:
                            fullscreen=!fullscreen;
                            if ( fullscreen ) {
                                SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
                                SDL_GetWindowSize(window,&screen_width,&screen_height);
                                SDL_SetWindowSize(window,screen_width,screen_height);
                            } else {
                                SDL_SetWindowFullscreen(window, 0);
                                SDL_SetWindowSize(window,min_screen_width,min_screen_height);
                                SDL_GetWindowSize(window,&screen_width,&screen_height);
                                SDL_SetWindowSize(window,screen_width,screen_height);
                            	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                            }
                            set_board(&board,screen_width,screen_height);
                            draw_background(renderer,&board,Text_row,Text_col,Text_move);
                            // Text move has to be rendered if WINDOWEVENT_RESIZED but only
                            if (prev_showed == showed) { 
                                render_text_from_int(renderer,font24, black, showed,screen_width*0.15,screen_height*0.1);
                            }
                            //reset position in pixel from new board size
                            for (size_t i=0;i<played;++i){
                                set_stone(stones+i,stones[i].linex,stones[i].liney,board,-1);
                            }
                            render_stones(renderer, stones ,img,wstone, bstone,showed);
                            break;
                        case SDLK_SPACE:
                        case SDLK_PAGEUP:
                            showed=(showed+1<=played) ? showed+1: played;
                            break;
                        case SDLK_r:
                        case SDLK_PAGEDOWN:
                            showed=((int) (showed-1)>=0) ? showed-1: 0;
                            draw_background(renderer,&board,Text_row,Text_col,Text_move);
                            render_stones(renderer, stones ,img,wstone, bstone,showed);
                            break;
                        case SDLK_q:
                            quit = 1;
                            break;
                    }
                    break;
                case SDL_QUIT :
                    quit = 1; 
                    break;
            }
        } // SDL_event 
        //check if we should rendered Text move number
        if (prev_showed != showed ) {
            prev_showed=showed;
            showed_to_be_rendered=true;
        }
        //display last stones sprite + marker
        render_last_stone(renderer, board, img,wstone,bstone, stones[showed-1], showed-1);
        // rendering move number
        if (showed_to_be_rendered) { 
            draw_background(renderer,&board,Text_row,Text_col,Text_move);
            render_stones(renderer, stones ,img, wstone, bstone,showed);
            render_text_from_int(renderer,font24, black, showed,screen_width*0.15,screen_height*0.1);
            showed_to_be_rendered=false;
        }
        // Update screen
        SDL_RenderPresent(renderer);
    } // quit loop
    // free memory Texture and Surfaces
    for (size_t i=0; i<N;i++) {
        SDL_DestroyTexture(Text_row[i]);
        SDL_DestroyTexture(Text_col[i]);
        SDL_FreeSurface(Surf_row[i]);
        SDL_FreeSurface(Surf_col[i]);
    }
    SDL_DestroyTexture(wstone); 
    SDL_DestroyTexture(bstone); 
    //Quit TTF related
    TTF_CloseFont(font16);
    TTF_CloseFont(font24);
    TTF_Quit();
    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    // Destroy window
    SDL_DestroyWindow(window);
    // Quit SDL
    SDL_Quit();
    return 0;
}
