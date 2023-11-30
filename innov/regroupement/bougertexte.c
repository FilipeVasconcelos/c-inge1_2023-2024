#include<stdio.h>
#include<time.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define TAILLE_LETTRE   30
#define MAX_LEN         256
#define TAILLE_DIC      50950

typedef struct{
    SDL_Rect hitbox;
    SDL_Surface* surfaceMessage;
    SDL_Texture* message;
    int M;
        
}motafficher;

int randmot(void){
    return   (rand()%TAILLE_DIC);
}

void bouger(SDL_Rect* h){
    h->x -=5;
}

void changerDeMOt(motafficher* a,SDL_Surface* b,SDL_Texture* c){ 
    a->surfaceMessage=b;
    a->message=c;
    a->M=0;
}

int main(void) {
    printf("inside main\n"); 
    printf("inside main\n"); 
    printf("inside main\n"); 
    printf("inside main\n"); 
    srand(time(NULL));
            
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur d'initialisation de la SDL : %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        SDL_Log("Erreur d'initialisation de SDL_ttf : %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Exemple SDL_ttf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        SDL_Log("Erreur de création de la fenêtre : %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur de création du renderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger une police TrueType
    TTF_Font* font = TTF_OpenFont("Police_OPENUP/arial.ttf", 100);
    if (!font) {
        SDL_Log("Erreur de chargement de la police : %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color color = {255, 255, 0};  // Couleur du texte (jaune)
    //FILE* file=fopen("/usr/share/dict/french","r");
    //FILE* file=fopen("/usr/share/dict/words","r");
    FILE* file=fopen("dico.txt","r");
    char mot[MAX_LEN];
    char mots[TAILLE_DIC][MAX_LEN];
    size_t nb=0;
    while(fgets(mot,MAX_LEN, file)){
        mot[strcspn(mot, "\n")] = '\0';
        strncpy(mots[nb],mot,strlen(mot)-1);
        printf("size of mots[%d] : %d %s\n",nb,strlen(mots[nb]),mots[nb]);
        nb++;
    }
    fclose(file);
    putchar('\n');
    printf("mots[0] : (size : %d) %s \n",strlen(mots[0]),mots[0]);
  
    Uint64 startTime = SDL_GetTicks64();
    SDL_Rect hitbox={SCREEN_WIDTH-strlen(mots[0])* TAILLE_LETTRE,100, strlen(mots[0])*TAILLE_LETTRE, 90};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(font,mots[0], color);
    //SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font,mots[0], color);
    SDL_Texture* message=SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_QUIT: 
                    quit = 1;
                    break;
            }
        }
        if (!((SDL_GetTicks64()-startTime)%3)) bouger(&hitbox);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, message, NULL, &hitbox);
        SDL_RenderPresent(renderer);
    } // while loop

    // Nettoyer
    SDL_DestroyTexture(message);
    SDL_FreeSurface(surfaceMessage); 
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
