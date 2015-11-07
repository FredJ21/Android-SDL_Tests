#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_TAILLE_X        1024
#define MAP_TAILLE_Y         768
#define APP_TITRE           "Test SDL 2"
#define false   0
#define true    1
#define bool    short int


int main( int argc, char* args[] )
{
    srand(time(NULL));

    printf("Hello Fred!\n");

    SDL_Log("Fred-->Hello SDL_LOG 2\n");

    /******************************************************************************************************************
                                                INIT SDL 2
    *******************************************************************************************************************/

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO) != 0 ) {  printf( "SDL_Init ERREUR ! SDL_GetError: %s\n", SDL_GetError() ); return -1; }


    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);
    int width = mode.w;
    int height = mode.h;

    // Création de la fenêtre
    SDL_Window *pWindow = NULL;

//    pWindow = SDL_CreateWindow( NULL , 0, 0, width, height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);

    pWindow = SDL_CreateWindow( APP_TITRE , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP );
    if(!pWindow) {                          printf( "SDL_Window ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;}


    // Création du Renderer
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!pRenderer) {                        printf( "SDL_Renderer ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;}

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // permet d'obtenir les redimensionnements plus doux.
  //  SDL_RenderSetLogicalSize(pRenderer, MAP_TAILLE_X, MAP_TAILLE_Y);


    SDL_Log("Chargement image\n");
    // Chargement de l'image
    SDL_Surface *pSurface = SDL_LoadBMP ("images/PetitBateau1.bmp");
    if(!pSurface) {
    	SDL_Log("Chargement ERROR\n");
    	printf( "SDL_Surface ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;
    }
    SDL_Log("Chargement ok\n");

    // Création de la texture (texture = surface dans le GPU)
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if(!pTexture) {                         printf( "SDL_Texture ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;}


    SDL_RenderCopy (pRenderer, pTexture, NULL, NULL);
    //SDL_SetRenderDrawColor (pRenderer, 254, 0, 0, 0);
    //SDL_RenderFillRect(pRenderer, NULL);
    SDL_RenderPresent (pRenderer);


    bool flag_fin = false;

    int R = 0;
    int V = 0;
    int B = 0;
    int a = 0;

    while (!flag_fin) {




        /******************************************************************************************************************
                                                    GESTION DES EVENEMENTS
        *******************************************************************************************************************/
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            switch (event.type){

                case SDL_QUIT:
                    flag_fin = true;
                    break;

               case SDL_KEYDOWN:

                    switch( event.key.keysym.sym ){
                        case SDLK_ESCAPE:
                            flag_fin = true;
                            break;
                    }
                    break;
            }



        }

/*
        R = rand()%254;
        V = rand()%254;
        B = rand()%254;


        SDL_Log("%d %d %d ! %d\n", R, V, B, a);

        SDL_SetRenderDrawColor (pRenderer, R, V, B, 0);
        SDL_RenderFillRect(pRenderer, NULL);
*/
        //SDL_RenderPresent (pRenderer);

        SDL_Delay(100);

        a++;
        if (a>100) {
        	flag_fin = true;
        }


    }
    SDL_Log("Fred-->Fin\n");
    return 0;
}
