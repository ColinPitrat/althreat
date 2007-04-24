#ifndef _T3F_GRAPHICS
#define _T3F_GRAPHICS

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include "Configuration.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DEFAULT_BPP 24

// Initialise SDL
void InitSDL();

// Renvoi une surface contenant le texte "texte" ecrit en
// couleur "fgColor" dans la police de nom "font_face" de 
// taille "font_size" sur fond transparent
// Il est souvent plus judicieux de charger la police une 
// seule fois plutôt que d'utiliser cette fonction.
SDL_Surface *Texte(const char *texte, const char *font_face, short font_size, SDL_Color fgColor);

// Place un pixel de couleur "pixel" sur la surface "surface"
// à l'abscisse "x" et l'ordonnée "y"
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

// Renvoi la valeur du pixel situé à l'abscisse "x" et l'ordonnée "y"
// sur la surface "surface"
Uint32 getpixel(SDL_Surface *surface, int x, int y);

// Renvoi une surface contenant l'image demandée
SDL_Surface *loadImage(const char *fileName);

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
