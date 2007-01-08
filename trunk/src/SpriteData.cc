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
      std::cerr << "Erreur : Impossible de charger " << fichiers[i] << " : " << SDL_GetError() << std::endl;
      break;
    }

    Pictures[i] = SDL_DisplayFormatAlpha(tmp);
    if(Pictures[i] == NULL)
    {
      std::cerr << "Erreur : Impossible de charger " << fichiers[i] << " : " << SDL_GetError() << std::endl;
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
        std::cerr << "Erreur : L'image " << fichiers[i] << " ne fait pas la même taille que " << fichiers[0] << std::endl;
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
      std::cerr << "Erreur : Une image passée pour une animation est vide\n";
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
        std::cerr << "Erreur : Les images d'un sprite ne font pas toutes la même taille." << std::endl;
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
    std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;

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
      std::cerr << "Erreur : Impossible de charger " << prefix+filename2 << " : " << SDL_GetError() << std::endl;
      break;
    }

    Pictures[i] = SDL_DisplayFormatAlpha(tmp);
    if(Pictures[i] == NULL)
    {
      std::cerr << "Erreur : Impossible de charger " << prefix+filename2 << " : " << SDL_GetError() << std::endl;
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
        std::cerr << "Erreur : L'image " << prefix+filename2 << " du sprite " << filename << " ne fait pas la taille prévue." << std::endl;
        break;
      }
    }
  }

  if(fichier.fail())
    std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;
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

