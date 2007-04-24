#ifndef _T3F_SPRITE_DATA
#define _T3F_SPRITE_DATA

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>

class SpriteData
{
public:
  SpriteData(int num, char **fichiers, int delay);
  SpriteData(int num, SDL_Surface **images, int delay);
  SpriteData(std::string filename, std::string prefix="");
  ~SpriteData();

  void setColorKey(Uint32 flags, Uint32 key);
  void setAlpha(Uint32 flags, Uint32 alpha);
  int getNbPictures() { return nbPict; };
  int getDelay() { return delay; };
  int getWidth() { return width; };
  int getHeight() { return height; };
  SDL_Surface *getPicture(int num) { return Pictures[num % nbPict]; };

private:
  SDL_Surface **Pictures;
  int nbPict;
  int delay;
  int width, height;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
