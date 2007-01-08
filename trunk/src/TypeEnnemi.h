#ifndef _ALTHREAT_TYPEENNEMI
#define _ALTHREAT_TYPEENNEMI

#include <fstream>
#include <iostream>
#include <string>
#include <SDL/SDL_mixer.h>
#include "Sprite.h"
#include "Configuration.h"

class TypeEnnemi
{
public:
  TypeEnnemi(std::string filename);
  ~TypeEnnemi();

  unsigned int getEnergieMax() { return energieMax; };
  unsigned int getValeur() { return score; };
  unsigned int getDegats() { return degats; };
  Mix_Chunk *getSon() { return son; };
  SpriteData* getSkin() { return skin; };
  SpriteData* getDestructSkin() { return destructSkin; };

private:
  unsigned int energieMax;
  unsigned int score;
  unsigned int degats;
  Mix_Chunk *son;
  SpriteData* skin;
  SpriteData* destructSkin;
};

#endif
