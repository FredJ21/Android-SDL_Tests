#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "anim.h"

/*****************************************************************
*****************************************************************/
void init_animation(t_animation *a, SDL_Renderer *r)  {


    SDL_Surface *pSurface_tmp = SDL_LoadBMP ( a->file );
            if(!pSurface_tmp) { printf( "LOAD BMP ERROR : %s\n", SDL_GetError() ); exit(1);}

    SDL_SetColorKey(pSurface_tmp, SDL_TRUE, SDL_MapRGB(pSurface_tmp->format, 255, 255, 255));

    a->texture = SDL_CreateTextureFromSurface(r, pSurface_tmp);
        if(!a->texture) { printf( "SDL_Texture ERREUR! SDL_GetError: %s\n", SDL_GetError() ); exit(1);}

    SDL_FreeSurface(pSurface_tmp);

}
/*****************************************************************
*****************************************************************/
t_sprite    *init_sprite(t_animation *a){
	t_sprite *s = (t_sprite*)malloc(sizeof(t_sprite));

	s->x = 0;
	s->y = 0;
	s->dx = 0;
	s->dy = 0;
	s->img_current = 0;
	s->compte_tour = 0;
	s->nb_tour = a->nb_tour;
	s->direction = 0;
	s->anim = a;
	s->is_actif = true;
	s->is_arrive = false;
	s->in_the_map = false;
	s->time_before_ativiation = 0;
	s->visible = 255;

	return s;
}
/*****************************************************************
*****************************************************************/
t_sprite*   create_Enemy( int position, int A, int B, t_animation *ANIM, float Frequence) {
                            // position --> en haut, à droite, en bas, à gauche
                            // A & B    --> délimite la zone de création , entre A et B
                            // *ANIM    --> pointeur sur l'annimation
                            // Frequence --> Frequence de démarrage entre chaque ennemi

 //           ENNEMI[e] = init_sprite( MAP_TAILLE_X + 3, (rand()%(MAP_TAILLE_Y-200))+100, 2, 2, 5, LEFT, ANIM, (GAME_FPS * e*3)+1 );
//    srand(time(NULL));

	t_sprite *s = (t_sprite*)malloc(sizeof(t_sprite));

    switch ( position ){

        case UP:
                s->x = (rand()%(B-A)) + A;
                s->y = 0 - ANIM->ty/2 ;
                s->direction = DOWN;
                break;
        case RIGHT:
                s->x = MAP_TAILLE_X + ANIM->tx/2 ;
                s->y = (rand()%(B-A)) + A;;
                s->direction = LEFT;
                break;
        case DOWN:
                s->x = (rand()%(B-A)) + A;;
                s->y = MAP_TAILLE_Y + ANIM->ty/2 ;
                s->direction = UP;
                break;
        case LEFT:
                s->x = 0 - ANIM->tx/2 ;
                s->y = (rand()%(B-A)) + A;;
                s->direction = RIGHT;
                break;
    }

	s->dx = ANIM->vitesse;
	s->dy = ANIM->vitesse;
	s->img_current = 0;
	s->compte_tour = 0;
	s->nb_tour = ANIM->nb_tour;
	s->anim = ANIM;
	s->is_actif = false;
	s->is_arrive = false;
	s->in_the_map = false;
	s->time_before_ativiation = (GAME_FPS * Frequence) +1 ;
	s->visible = 255;

	if (s->time_before_ativiation == 0 ) { s->is_actif = true; }

	return s;
}
/*****************************************************************
*****************************************************************/
void anime_sprite(t_sprite*s){
	if (s->is_actif == true ) {
      s->compte_tour++;
      if (s->compte_tour > s->nb_tour) {

        s->img_current = (s->img_current + 1)%s->anim->nb_img_by_dir;

		s->compte_tour = 0;
	  }
	}
}
/*****************************************************************
*****************************************************************/
void affiche_sprite(SDL_Renderer *r, t_sprite *s){
    SDL_Rect Src;
    SDL_Rect Dst;

    Src.x = s->img_current * s->anim->tx;
    Src.y = s->direction * s->anim->ty;
    Src.w = s->anim->tx;
    Src.h = s->anim->ty;

    Dst.x = s->x - s->anim->tx/2;     // permet de center le sprite sur les coordonnées
    Dst.y = s->y - s->anim->ty/2;
    Dst.w = s->anim->tx;
    Dst.h = s->anim->ty;


    // mode transparent de l'animimation à l'arrivé
    if (s->is_actif == false && s->visible > 0 && s->time_before_ativiation == 0 ) {
        s->visible -=10 ;
    }
    if (s->visible < 0 ) {
        s->visible = 0;
    }
    SDL_SetTextureAlphaMod (s->anim->texture, s->visible);
 //   SDL_SetRenderDrawColor (r, 254, 0, 0, 50);

    // Affichage
    SDL_RenderCopy ( r, s->anim->texture , &Src, &Dst);

}
/*****************************************************************
*****************************************************************/
void place_sprite (t_sprite *p, int x, int y) {

    p->x = TILE_TAILLE_X * x;
    p->y = TILE_TAILLE_Y * y;
}
/*****************************************************************
*****************************************************************/
void destroy_sprite(t_sprite **s) {

    free(*s);
    *s = NULL;
}
/*****************************************************************
*****************************************************************/
