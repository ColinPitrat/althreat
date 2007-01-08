#include "Bonus.h"

Bonus::Bonus(TypeBonus *settype, int setpos, double sety, double setdx, double setdy)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : bonus" << std::endl;
  SDL_Surface *Screen = SDL_GetVideoSurface();
  type = settype;
  skin = new Sprite(type->getSkin());
  pos = setpos;
  if(pos < 0)
    skin->setPos(Screen->w + pos, sety);
  else
    skin->setPos(Screen->w, sety);
  dx = -setdx;
  dy = setdy;
  dead = false;
}

Bonus::~Bonus()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : bonus" << std::endl;
  delete skin;
}

void Bonus::detruire()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(!configuration->nosound() && type->getSon())
  {
    if(Mix_PlayChannel(-1, type->getSon(), 0) == -1) 
    {
      // Peut-�tre une erreur critique, ou simplement l'abscence de channel libre
      if(configuration->verbose())
        std::cerr << "Erreur : Impossible de jouer un son : " << Mix_GetError() << std::endl;
    }
  }
  dead = true;
}

void Bonus::afficher()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  skin->afficher(Screen);
}

bool Bonus::horsEcran()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  const SDL_Rect *pos = skin->getPos();
  return (pos->x + (Sint16)pos->w < 0) || (pos->y + (Sint16)pos->h < 0) || (pos->y > Screen->h);
}