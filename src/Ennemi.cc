#include "Ennemi.h"

Ennemi::Ennemi(TypeEnnemi *settype, int setpos, double sety, double setdx, double setdy)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : ennemi" << std::endl;
  SDL_Surface *Screen = SDL_GetVideoSurface();
  type = settype;
  energie = type->getEnergieMax();
  skin = new Sprite(type->getSkin());
  destruct = new Sprite(type->getDestructSkin());
  pos = setpos;
  if(pos < 0)
    skin->setPos(Screen->w + pos, sety);
  else
    skin->setPos(Screen->w, sety);
  dx = -setdx;
  dy = setdy;
}

Ennemi::~Ennemi()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : ennemi" << std::endl;
  delete skin;
  delete destruct;
}

unsigned int Ennemi::endommager(unsigned int degats)
{
  if((signed int)energie - (signed int)degats > 0)
  {
    energie -= degats;
    return 0;
  }
  else
  {
    this->detruire();
    return type->getValeur();
  }
}

void Ennemi::detruire()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(!configuration->nosound() && type->getSon())
  {
    if(Mix_PlayChannel(-1, type->getSon(), 0) == -1) 
    {
      // Peut-être une erreur critique, ou simplement l'abscence de channel libre
      if(configuration->verbose())
        std::cerr << "Erreur : Impossible de jouer un son : " << Mix_GetError() << std::endl;
    }
  }
  energie = 0;
  const SDL_Rect *pos = skin->getPos();
  destruct->setPos(pos->x, pos->y);
  destruct->reset();
}

void Ennemi::afficher()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  if(energie > 0)
    skin->afficher(Screen);
  else
    destruct->afficher(Screen, false);
}

bool Ennemi::horsEcran()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  const SDL_Rect *pos = skin->getPos();
  return (pos->x + (Sint16)pos->w < 0) || (pos->y + (Sint16)pos->h < 0) || (pos->y > Screen->h);
}

void Ennemi::animer(double delay)
{
  if(energie > 0)
  {
    double deltaX = ((double)delay) * dx;
    double deltaY = ((double)delay) * dy;
    skin->deplacer(deltaX, deltaY); 
  }
}
