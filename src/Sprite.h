#ifndef _T3F_SPRITE
#define _T3F_SPRITE

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include "graphics.h"
#include "SpriteData.h"

class Sprite
{
public:
  Sprite(int num, char **fichiers, int delay);
  Sprite(int num, SDL_Surface **images, int delay);
  Sprite(std::string filename, std::string prefix="");
  Sprite(SpriteData *setdata);
  ~Sprite();

  void setColorKey(Uint32 flags, Uint32 key) { if(data) data->setColorKey(flags, key); };
  void setAlpha(Uint32 flags, Uint32 alpha) { if(data) data->setAlpha(flags, alpha); };
  void setLimites(SDL_Rect *Rect);
  void reset() { finished = false; nextTime = SDL_GetTicks(); animPos = 0; };
  void setPos(double setx, double sety) { x = setx; y = sety; };

  bool fini() { return finished; };
  int getNbPictures() { if(data) return data->getNbPictures(); else return 0; };
  SDL_Surface *getPicture() { animer(); if(data) return data->getPicture(animPos); else return NULL; };
  SDL_Surface *getPicture(int num) { if(data) return data->getPicture(num); else return NULL; };
  const SDL_Rect *getPos() { return &pos; };

  void deplacer(double dx, double dy);
  void animer();
  void afficher(SDL_Surface *Screen, bool hideIfFinished=true);
  bool collision(Sprite *ennemi);
  bool isHBlocked() { return hblocked; };
  bool isVBlocked() { return vblocked; };

private:
  /*SDL_Surface **Pictures;
  int nbPict;
  int delay;*/
  SpriteData *data;
  SDL_Rect pos;
  double x, y;
  SDL_Rect limites;
  int animPos;
  bool finished;
  bool limited;
  bool sharedData;
  bool hblocked;
  bool vblocked;
  Uint32 nextTime;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
