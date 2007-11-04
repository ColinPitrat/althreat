#include "Level.h"

Level::Level(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : level" << std::endl;
  pos = 0;
  // Initialisation des étoiles
  {
    SDL_Surface *Screen = SDL_GetVideoSurface();
    for(unsigned int i = 0; i < NB_ETOILES; i++)
    {
      etoiles[i].x = random() % Screen->w; 
      etoiles[i].y = random() % Screen->h; 
      etoiles[i].dx = (double)(ETOILES_VIT_MIN + (random() % (ETOILES_VIT_MAX - ETOILES_VIT_MIN))) / (double)ETOILES_VIT_RATIO;
      etoiles[i].color = SDL_MapRGB(Screen->format, random() % 255, random() % 255, random() % 255); 
    }
  }

  std::ifstream fichier;
  std::string nomMusique;
  fichier.open((configuration->getDataDir() + filename).c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;

  fichier >> longueur >> nomMusique >> nbTypesEnnemis;
  music_file = configuration->getDataDir() + nomMusique;
  musique = Mix_LoadMUS((music_file).c_str());
  typesEnnemis = new TypeEnnemi*[nbTypesEnnemis];
  for(unsigned int i = 0; i < nbTypesEnnemis; i++)
  {
    std::string nom;
    fichier >> nom;
    typesEnnemis[i] = new TypeEnnemi(nom);
  }

  unsigned int nbEnnemis;
  fichier >> nbEnnemis;
  for(unsigned int i = 0; i < nbEnnemis; i++)
  {
    unsigned int type; 
    int position;
    double y, dx, dy;
    fichier >> type >> position >> y >> dx >> dy;
    ennemis.push_back(new Ennemi(typesEnnemis[type - 1], position, y, dx, dy));
  }

  fichier >> nbTypesBonus;
  typesBonus = new TypeBonus*[nbTypesBonus];
  for(unsigned int i = 0; i < nbTypesBonus; i++)
  {
    std::string nom;
    fichier >> nom;
    typesBonus[i] = new TypeBonus(nom);
  }
  // TODO : Une gestion des bonus un peu mieux !
  unsigned int nbBonus;
  fichier >> nbBonus;
  for(unsigned int i = 0; i < nbBonus; i++)
  {
    unsigned int type; 
    int position;
    double y, dx, dy;
    fichier >> type >> position >> y >> dx >> dy;
    bonus.push_back(new Bonus(typesBonus[type - 1], position, y, dx, dy));
  }

  if(fichier.fail())
    std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

  fichier.close();
}

Level::~Level()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : level" << std::endl;
  if(musique)
  {
    Mix_FreeMusic(musique);
    musique = NULL;
  }
  // Detruit les types d'ennemis
  for(unsigned int i = 0; i < nbTypesEnnemis; i++)
    delete typesEnnemis[i];
  delete[] typesEnnemis;
  // Detruit les types de bonus
  for(unsigned int i = 0; i < nbTypesBonus; i++)
    delete typesBonus[i];
  delete[] typesBonus;
  // Detruit les ennemis
  {
    std::list<Ennemi*>::iterator it = ennemis.begin();
    while(it != ennemis.end())
    {
      delete *it;
      it = ennemis.erase(it);
    }
  }
  // Detruit les bonus
  {
    std::list<Bonus*>::iterator it = bonus.begin();
    while(it != bonus.end())
    {
      delete *it;
      it = bonus.erase(it);
    }
  }
  // Detruit les projectiles
  {
    std::list<Projectile*>::iterator it = projectiles.begin();
    while(it != projectiles.end())
    {
      delete *it;
      it = projectiles.erase(it);
    }
  }
}

void Level::playMusic()
{
  if(!musique || (Mix_PlayMusic(musique, -1) == -1))
  {
    // L'erreur n'est pas forcement fatale
    std::cerr << "Impossible de jouer la musique du menu : " << Mix_GetError() << std::endl;
  }
}

void Level::stopMusic()
{
  Mix_HaltMusic();
}

void Level::afficher()
{
  // Affiche les étoiles
  SDL_Surface *Screen = SDL_GetVideoSurface();
  for(unsigned int i = 0; i < NB_ETOILES; i++)
    putpixel(Screen, (int)etoiles[i].x, (int)etoiles[i].y, etoiles[i].color);

  // Affiche les ennemis
  {
    std::list<Ennemi*>::iterator it = ennemis.begin();
    while((it != ennemis.end()) && ((*it)->getPos() < pos))
    {
      (*it)->afficher();
      it++;
    }
  }

  // Affiche les bonus
  {
    std::list<Bonus*>::iterator it = bonus.begin();
    while((it != bonus.end()) && ((*it)->getPos() < pos))
    {
      (*it)->afficher();
      it++;
    }
  }

  // Affiche les projectiles
  {
    std::list<Projectile*>::iterator it = projectiles.begin();
    while(it != projectiles.end())
    {
      (*it)->afficher();
      it++;
    }
  }
}

void Level::animer(double delay)
{
  pos += delay;
  // Anime les étoiles
  {
    SDL_Surface *Screen = SDL_GetVideoSurface();
    for(unsigned int i = 0; i < NB_ETOILES; i++)
    {
      etoiles[i].x -= etoiles[i].dx * delay;
      if(etoiles[i].x <= 0)
      {
        etoiles[i].x = Screen->w; 
        etoiles[i].y = random() % Screen->h; 
        etoiles[i].dx = (ETOILES_VIT_MIN + random() % (ETOILES_VIT_MAX - ETOILES_VIT_MIN)) / ETOILES_VIT_RATIO;
        etoiles[i].color = SDL_MapRGB(Screen->format, random() % 255, random() % 255, random() % 255); 
      }
    }
  }

  // Anime les ennemis
  {
    std::list<Ennemi*>::iterator it = ennemis.begin();
    while((it != ennemis.end()) && ((*it)->getPos() < pos))
    {
      (*it)->animer(delay);
      if((*it)->horsEcran() || (*it)->isDead())
      {
        delete *it;
        it = ennemis.erase(it);
      }
      else
      {
        it++;
      }
    }
  }

  // Anime les bonus
  {
    std::list<Bonus*>::iterator it = bonus.begin();
    while((it != bonus.end()) && ((*it)->getPos() < pos))
    {
      (*it)->animer(delay);
      if((*it)->horsEcran() || (*it)->isDead())
      {
        delete *it;
        it = bonus.erase(it);
      }
      else
      {
        it++;
      }
    }
  }

  // Anime les projectiles
  {
    std::list<Projectile*>::iterator it = projectiles.begin();
    while(it != projectiles.end())
    {
      (*it)->animer(delay);
      if((*it)->horsEcran() || (*it)->isDead())
      {
        delete *it;
        it = projectiles.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
