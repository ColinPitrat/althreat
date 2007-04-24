#include "Sprite.h"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

Sprite::Sprite(int num, char **fichiers, int d)
{
  sharedData = false;
  finished = false;
  limited = false;
  animPos = 0;
  x = 0;
  y = 0;

  data = new SpriteData(num, fichiers, d);
  if(data)
  {
    pos.w = data->getWidth();
    pos.h = data->getHeight();
    nextTime = SDL_GetTicks() + data->getDelay();
  }
  else
  {
    std::cerr << "Erreur lors du chargement du contenu du sprite.\n";
  }
}

Sprite::Sprite(int num, SDL_Surface **images, int d)
{
  sharedData = false;
  finished = false;
  limited = false;
  animPos = 0;
  x = 0;
  y = 0;

  data = new SpriteData(num, images, d);
  if(data)
  {
    pos.w = data->getWidth();
    pos.h = data->getHeight();
    nextTime = SDL_GetTicks() + data->getDelay();
  }
  else
  {
    std::cerr << "Erreur lors du chargement du contenu du sprite.\n";
  }
}

Sprite::Sprite(std::string filename, std::string prefix)
{
  sharedData = false;
  finished = false;
  limited = false;
  animPos = 0;
  x = 0;
  y = 0;

  data = new SpriteData(filename, prefix);
  if(data)
  {
    pos.w = data->getWidth();
    pos.h = data->getHeight();
    nextTime = SDL_GetTicks() + data->getDelay();
  }
  else
  {
    std::cerr << "Erreur lors du chargement du contenu du sprite.\n";
  }
}

Sprite::Sprite(SpriteData* setdata)
{
  sharedData = true;
  finished = false;
  limited = false;
  animPos = 0;
  x = 0;
  y = 0;

  data = setdata;
  if(data)
  {
    pos.w = data->getWidth();
    pos.h = data->getHeight();
    nextTime = SDL_GetTicks() + data->getDelay();
  }
  else
  {
    std::cerr << "Erreur: le sprite est vide.\n";
  }
}

Sprite::~Sprite()
{
  if(!sharedData)
    delete data;
}

void Sprite::animer()
{
  if(data)
  {
    while(SDL_GetTicks() >= nextTime)
    {
      animPos++;
      if(animPos >= data->getNbPictures())
      {
        animPos -= data->getNbPictures();
        finished = true;
      }
      nextTime += data->getDelay();
    }
  }
}

void Sprite::setLimites(SDL_Rect *Rect)
{
  if(Rect)
  {
    limited = true;
    limites.x = Rect->x;
    limites.y = Rect->y;
    limites.w = Rect->w;
    limites.h = Rect->h;
  }
  else
  {
    limited = false;
  }
}

void Sprite::deplacer(double dx, double dy)
{
  x += dx;
  y += dy;
  pos.x = (int) x;
  pos.y = (int) y;
  if(limited)
  {
    hblocked = vblocked = false;
    if( pos.x + pos.w > limites.w + limites.x)
    {
      x = limites.x + limites.w - pos.w;
      hblocked = true;
    }
    if( pos.x < limites.x)
    {
      x = limites.x;
      hblocked = true;
    }
    if( pos.y + pos.h > limites.h + limites.y)
    {
      y = limites.y + limites.h - pos.h;
      vblocked = true;
    }
    if( pos.y < limites.y )
    {
      y = limites.y;
      vblocked = true;
    }
  }
}

void Sprite::afficher(SDL_Surface *Screen, bool showIfFinished)
{
  pos.x = (int) x;
  pos.y = (int) y;
  if(data)
  {
    animer(); 
    if(showIfFinished || !finished)
    {
      // Il faut utiliser une copie du rectangle car SDL_BlitSurface peut le modifier
      SDL_Rect temp;
      temp.x = pos.x; temp.y = pos.y; temp.w = pos.w; temp.h = pos.h;
      SDL_BlitSurface(data->getPicture(animPos), NULL, Screen, &temp); 
    }
  }
}

bool Sprite::collision(Sprite *ennemi)
{
  if(data)
  {
    const SDL_Rect *ennemiPos = ennemi->getPos();
    if(!((ennemiPos->x > pos.x + pos.w) || (ennemiPos->y > pos.y + pos.h) || (pos.x > ennemiPos->x + ennemiPos->w) || (pos.y > ennemiPos->y + ennemiPos->h)))
    {
      for(int i = MAX(ennemiPos->x, pos.x); i < MIN(ennemiPos->x + ennemiPos->w,pos.x + pos.w); i++)
      {
        for(int j = MAX(ennemiPos->y, pos.y); j < MIN(ennemiPos->y + ennemiPos->h, pos.y + pos.h); j++)
        {
          if(getpixel(ennemi->getPicture(), i - ennemiPos->x, j - ennemiPos->y) && getpixel(this->getPicture(), i - pos.x, j - pos.y))
            return true;
        }
      }
    }
  }
  return false;
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
