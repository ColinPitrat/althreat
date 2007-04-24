#include "Projectile.h"

Projectile::Projectile(Arme *settype, double initx, double inity, double initdx, double initdy)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : projectile" << std::endl;
  type = settype;
  skin = new Sprite(configuration->getDataDir() + type->getSkin(), configuration->getDataDir());
  destruct = new Sprite(configuration->getDataDir() + type->getDestructSkin(), configuration->getDataDir());
  skin->setPos(initx + type->getX(), inity + type->getY());
  explose = false;
  if(type->isLumique())
  {
    dx = type->getDx();
    dy = type->getDy();
  }
  else
  {
    dx = initdx + type->getDx();
    dy = initdy + type->getDy();
  }
  if(!configuration->nosound() && type->getSon())
  {
    if(Mix_PlayChannel(-1, type->getSon(), 0) == -1) 
    {
      // Peut-être une erreur critique, ou simplement l'abscence de channel libre
      if(configuration->verbose())
        std::cerr << "Erreur : Impossible de jouer un son : " << Mix_GetError() << std::endl;
    }
  }
}

Projectile::~Projectile()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : projectile" << std::endl;
  delete skin;
  delete destruct;
}

bool Projectile::horsEcran()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  const SDL_Rect *rect = skin->getPos();
  return (rect->x > Screen->w) || (rect->x + (Sint16)rect->w < 0) || (rect->y > Screen->h) || (rect->y + (Sint16)rect->h < 0);
}

void Projectile::afficher()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  if(explose)
    destruct->afficher(Screen, false);
  else
    skin->afficher(Screen);
}

void Projectile::detruire()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(!configuration->nosound() && type->getSonDestruct())
  {
    if(Mix_PlayChannel(-1, type->getSonDestruct(), 0) == -1) 
    {
      // Peut-être une erreur critique, ou simplement l'abscence de channel libre
      if(configuration->verbose())
        std::cerr << "Erreur : Impossible de jouer un son : " << Mix_GetError() << std::endl;
    }
  }
  explose = true; 
  const SDL_Rect *pos = skin->getPos();
  destruct->setPos(pos->x, pos->y);
  destruct->reset();
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
