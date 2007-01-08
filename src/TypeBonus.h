#ifndef _ALTHREAT_TYPEBONUS
#define _ALTHREAT_TYPEBONUS

#include <fstream>
#include <iostream>
#include <string>
#include <SDL/SDL_mixer.h>
#include "Sprite.h"
#include "Configuration.h"

class BonusAction
{
  public:
    std::string objet;
    std::string action;
    int nbParams;
    std::string *parametres;
};

class TypeBonus
{
public:
  TypeBonus(std::string filename);
  ~TypeBonus();

  Mix_Chunk *getSon() { return son; };
  SpriteData* getSkin() { return skin; };
  BonusAction* getActions() { return actions; };
  int getNbActions() { return nbActions; };

private:
  Mix_Chunk *son;
  SpriteData* skin;
  int nbActions;
  BonusAction* actions;
};

#endif
