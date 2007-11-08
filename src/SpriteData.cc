#include "SpriteData.h"

SpriteData::SpriteData(int num, char **fichiers, int d)
{
  nbPict = num;
  delay = d;
  Pictures = new SDL_Surface*[nbPict];
  int i=0;

  SDL_Surface *tmp;
  // On charge toutes les images
  for(i=0; i<nbPict; i++)
  {
    //Pictures[i] = IMG_Load(fichiers[i]);
    tmp = IMG_Load(fichiers[i]);
    if(tmp == NULL)
    {
      std::cerr << "Error: Impossible to load " << fichiers[i] << " : " << SDL_GetError() << std::endl;
      break;
    }

    Pictures[i] = SDL_DisplayFormatAlpha(tmp);
    if(Pictures[i] == NULL)
    {
      std::cerr << "Error : Impossible to load " << fichiers[i] << " : " << SDL_GetError() << std::endl;
      break;
    }
    SDL_FreeSurface(tmp);
    if(i == 0)
    {
      width = Pictures[0]->w;
      height = Pictures[0]->h;
    }
    else
    {
      if(Pictures[i]->w != width || Pictures[i]->h != height)
      {
        std::cerr << "Error : Picture " << fichiers[i] << " isn't the same size as " << fichiers[0] << std::endl;
        break;
      }
    }
  }
  nbPict = i;
}

SpriteData::SpriteData(int num, SDL_Surface **images, int d)
{
  nbPict = num;
  delay = d;
  Pictures = new SDL_Surface*[nbPict];
  int i=0;

  // On charge toutes les images
  for(i=0; i<nbPict; i++)
  {
    Pictures[i] = SDL_DisplayFormatAlpha(images[i]);
    if(Pictures[i] == NULL)
    {
      std::cerr << "Error : A picture given for an animation is empty\n";
      break;
    }
    if(i == 0)
    {
      width = Pictures[0]->w;
      height = Pictures[0]->h;
    }
    else
    {
      if(Pictures[i]->w != width || Pictures[i]->h != height)
      {
        std::cerr << "Error: Pictures of a sprite aren't all of the same size." << std::endl;
        break;
      }
    }
  }
  nbPict = i;
}

SpriteData::SpriteData(std::string filename, std::string prefix)
{
  std::ifstream fichier;
  fichier.open(filename.c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Error: Impossible to open a file " << filename << std::endl;

  fichier >> nbPict >> delay;

  Pictures = new SDL_Surface*[nbPict];
  SDL_Surface *tmp;
  for(int i = 0; i < nbPict; i++)
  {
    std::string filename2;
    fichier >> filename2;

    //Pictures[i] = IMG_Load(filename2.c_str());
    tmp = IMG_Load((prefix + filename2).c_str());
    if(tmp == NULL)
    {
      std::cerr << "Error: Impossible to load " << prefix+filename2 << " : " << SDL_GetError() << std::endl;
      break;
    }

    Pictures[i] = SDL_DisplayFormatAlpha(tmp);
    if(Pictures[i] == NULL)
    {
      std::cerr << "Error: Impossible to load " << prefix+filename2 << " : " << SDL_GetError() << std::endl;
      break;
    }
    SDL_FreeSurface(tmp);
    if(i == 0)
    {
      width = Pictures[0]->w;
      height = Pictures[0]->h;
    }
    else
    {
      if(Pictures[i]->w != width || Pictures[i]->h != height)
      {
        std::cerr << "Error : Picture " << prefix+filename2 << " of sprite " << filename << " is not the good size." << std::endl;
        break;
      }
    }
  }

  if(fichier.fail())
    std::cerr << "Error while reading file " << filename << std::endl;
}

SpriteData::~SpriteData()
{
  for(int i = 0; i < nbPict; i++)
  {
    SDL_FreeSurface(Pictures[i]);
  }
  delete[] Pictures;
}

void SpriteData::setColorKey(Uint32 flags, Uint32 key)
{
  for(int i=0; i<nbPict; i++)
  {
    SDL_SetColorKey(Pictures[i], flags, key);
  }
}

void SpriteData::setAlpha(Uint32 flags, Uint32 alpha)
{
  for(int i=0; i<nbPict; i++)
  {
    SDL_SetAlpha(Pictures[i], flags, alpha);
  }
}


// vim:shiftwidth=2:shiftround:expandtab:cindent
