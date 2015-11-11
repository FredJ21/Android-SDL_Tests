#include <SDL.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "anim.h"

#define MAP_TAILLE_X        1024
#define MAP_TAILLE_Y         768
#define false   0
#define true    1
#define bool    short int

#define GAME_FPS              30       // a repasser à 30

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
    SDL_RenderSetLogicalSize(pRenderer, MAP_TAILLE_X, MAP_TAILLE_Y);


    SDL_Log("Chargement image\n");
    // Chargement de l'image
    SDL_Surface *pSurface = SDL_LoadBMP (START_IMG);
    if(!pSurface) {
    	SDL_Log("Chargement image ERROR\n");
    	printf( "SDL_Surface ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;
    }

    // Création de la texture (texture = surface dans le GPU)
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if(!pTexture) {                         printf( "SDL_Texture ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;}

    /******************************************************************************************************************
                                                INIT SDL 2 TTF
    *******************************************************************************************************************/
    // Initialize SDL TTF
    if( TTF_Init() != 0 ) {  printf( "TTF_Init ERREUR ! SDL_GetError: %s\n", SDL_GetError() ); return -1; }

    // pour le titre de chaque level
    TTF_Font *police_level_titre;
    police_level_titre = TTF_OpenFont(POLICE_LEVEL_TITRE, POLICE_LEVEL_TITRE_SIZE);
    if(!police_level_titre) {                  printf( "TTF_OpenFont ERREUR! SDL_GetError: %s\n", SDL_GetError() ); return -1;}


    SDL_Surface *texte = NULL;
    SDL_Texture     *pTexture_MAP_Titre;

    SDL_Color couleur = {200, 100, 100, 0};

    SDL_Rect        Titre_position_src;
    SDL_Rect        Titre_position_dst;

    texte = TTF_RenderText_Blended(police_level_titre, "Hello Fred !", couleur);
    Titre_position_src.x = 0;
    Titre_position_src.y = 0;
    Titre_position_src.h = texte->h;
    Titre_position_src.w = texte->w;
    Titre_position_dst.x = 0;
    Titre_position_dst.y = MAP_TAILLE_Y/2;
    Titre_position_dst.h = texte->h;
    Titre_position_dst.w = texte->w;

    // Création de la texture pour le texte
    pTexture_MAP_Titre = SDL_CreateTextureFromSurface(pRenderer, texte);
    if(!pTexture_MAP_Titre) {                  printf( "SDL Texture ERREUR! SDL_GetError: %s\n", SDL_GetError() ); exit(-1);}

    SDL_FreeSurface(texte);


    /******************************************************************************************************************
                                                VARIABLES
    *******************************************************************************************************************/
    time_t t_Avant_Traitement;          // permet de gérer les fps
    time_t t_Apres_Traitement;
    Uint32  game_sleep;

    bool flag_fin = false;

    int R = 0;
    int V = 0;
    int B = 0;
    int a = 0;

    int text_move_x = 10;
    int text_move_y = 10;

    /******************************************************************************************************************
                                                INIT GAME
    *******************************************************************************************************************/
    /** ANIMATION **/

    t_animation DRAPEAU = { "images/Flag.bmp", 31, 40, 11, 11, 11, NULL, 3, 1 };
    init_animation( &DRAPEAU, pRenderer);

    /* SPRITE ARRIVE */
    t_sprite *ARRIVE;
    ARRIVE = init_sprite (&DRAPEAU);


    place_sprite(ARRIVE, 11, 12);


    while (!flag_fin) {

        t_Avant_Traitement = clock();

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

        /******************************************************************************************************************
                                                    Modification position du texte
        *******************************************************************************************************************/
        Titre_position_dst.x += text_move_x;
        Titre_position_dst.y += text_move_y;

        if (Titre_position_dst.x > MAP_TAILLE_X - Titre_position_dst.w) { text_move_x = -10; }
        if (Titre_position_dst.y > MAP_TAILLE_Y - Titre_position_dst.h) { text_move_y = -10; }
        if (Titre_position_dst.x < 0) { text_move_x = 10; }
        if (Titre_position_dst.y < 0) { text_move_y = 10; }


        /******************************************************************************************************************
                                                    AFFICHAGE
        *******************************************************************************************************************/
        SDL_RenderClear     (pRenderer);

        SDL_RenderCopy (pRenderer, pTexture, NULL, NULL);

        // TEXTE
        SDL_RenderCopy   (pRenderer, pTexture_MAP_Titre,  &Titre_position_src, &Titre_position_dst);

        // Affichage de l'arrivé
        anime_sprite(ARRIVE);
        affiche_sprite (pRenderer, ARRIVE);

        SDL_RenderPresent (pRenderer);


        /************************************************/
        /**   Calcul du temps de traitement et pause   **/
        /************************************************/
        t_Apres_Traitement = clock();
        //game_sleep = 1000 / GAME_FPS - (t_Apres_Traitement - t_Avant_Traitement);
        game_sleep = 1000 / GAME_FPS ;
        //SDL_Log("sleep : %d\n", game_sleep);

        SDL_Delay( game_sleep );

    }

    /******************************************************************************************************************
                                                    FIN
    *******************************************************************************************************************/
   SDL_Log("Fred-->Fin\n");

    destroy_sprite(&ARRIVE);

    SDL_DestroyTexture(pTexture);
    SDL_DestroyTexture(pTexture_MAP_Titre);
    SDL_FreeSurface(pSurface);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow( pWindow );

    SDL_Quit();

    return 0;
}
