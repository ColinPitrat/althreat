#include "Game.h"

Game::Game()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : game" << std::endl;
  level = NULL;
  vaisseau = new Vaisseau();
  numLevel = 0;
  vies = NB_VIES;
  score = 0;
}

Game::Game(bool practice = false)
{
  Configuration *configuration = Configuration::getConfiguration();
  SDL_Surface *Screen = SDL_GetVideoSurface();
  if(configuration->debug())
    std::cerr << "Construction : game" << std::endl;
  level = NULL;
  vaisseau = new Vaisseau();
  level = 0;
  if(practice)
  {
    View *view;
    view = View::getView();

    FocusContainer *cont = new FocusContainer();

    SDL_Rect liste_pos;
    liste_pos.x = 250; liste_pos.y = 200; liste_pos.w = 500; liste_pos.h = 300;
    Liste *liste = new Liste(liste_pos);
    liste->setFont((configuration->getDataDir() + "fonts/babelfish.ttf"), 30);
    std::ifstream fichier;
    unsigned int nbLevels;
    fichier.open((configuration->getDataDir() + "levels/levels.lst").c_str(), std::ios::in);
    fichier >> nbLevels;
    for(unsigned int i = 0; i < nbLevels; i++)
    {
      std::string level;
      std::stringbuf levelNameBuf;
      std::string levelName;
      fichier >> level;
      fichier.get(levelNameBuf);
      levelName = levelNameBuf.str();
      liste->addValue(new std::string(levelName));
    }
    cont->addWidget(liste);
    cont->focus(liste);
    view->addWidget(cont);
    bool choisi = false;
    SDL_Surface *Fond = loadImage((configuration->getDataDir() + "images/Choix.png").c_str());
    SDL_BlitSurface(Fond, NULL, Screen, NULL);
    while(!choisi)
    {
      view->afficher(Screen);
      SDL_Flip(Screen);
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        switch(event.type)
        {
          case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == configuration->touche(TOUCHE_ARME))
              choisi = true;
            break;
          case SDL_QUIT:
            exit(0);
          default:
            break;
        } 
      }
    }
    numLevel = liste->getIntValue();
  }
  vies = NB_VIES;
  score = 0;
}

Game::~Game()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : game" << std::endl;
  if(vaisseau)
    delete vaisseau;
  if(level)
    delete level;
}

bool Game::resetLevel(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(!configuration->nosound())
    level->stopMusic();
  if(level)
    delete level;
  level = NULL;

  std::ifstream levelsList;
  std::stringbuf levelNameBuf;
  std::string levelName;
  std::string levelFile;
  unsigned int nbLevels;

  // Recherche le fichier correspondant au prochain level dans la liste des levels
  {
    levelsList.open((configuration->getDataDir() + filename.c_str()).c_str(),std::ios::in);
    if(!levelsList)
      std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;

    levelsList >> nbLevels;

    if(levelsList.fail())
      std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

    if(numLevel > nbLevels)
      return false;

    for(unsigned int i = 0; i < numLevel; i++)
    {
      levelsList >> levelFile;
      levelsList.get(levelNameBuf);
      levelName = levelNameBuf.str();
    }

    if(levelsList.fail())
      std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

    levelsList.close();
  }
  level = new Level(levelFile);
  if(!configuration->nosound())
    level->playMusic();

  return true;
}

bool Game::nextLevel(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  numLevel++;
  if(!configuration->nosound())
    level->stopMusic();
  if(level)
    delete level;
  level = NULL;

  std::ifstream levelsList;
  std::stringbuf levelNameBuf;
  std::string levelName;
  std::string levelFile;
  unsigned int nbLevels;

  // Recherche le fichier correspondant au prochain level dans la liste des levels
  {
    levelsList.open((configuration->getDataDir() + filename.c_str()).c_str(),std::ios::in);
    if(!levelsList)
      std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;

    levelsList >> nbLevels;

    if(levelsList.fail())
      std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

    if(numLevel > nbLevels)
      return false;

    for(unsigned int i = 0; i < numLevel; i++)
    {
      levelsList >> levelFile;
      levelsList.get(levelNameBuf);
      levelName = levelNameBuf.str();
    }

    if(levelsList.fail())
      std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

    levelsList.close();
  }
  level = new Level(levelFile);
  if(!configuration->nosound())
    level->playMusic();

  return true;
}

void Game::afficher()
{
  Configuration *configuration = Configuration::getConfiguration();
  level->afficher();
  vaisseau->afficher();

  // Affiche les stats
  SDL_Surface *Screen = SDL_GetVideoSurface();
  std::string infos;
  std::ostringstream oss (std::ostringstream::out);

  oss << "Level : " << this->numLevel << "   Score : " << this->score << "   Vies : " << this->vies << "   Energie : ";
  infos = oss.str();

  SDL_Color color;
  color.r = 255; color.g = 255; color.b = 0;
  SDL_Surface *Infos = Texte(infos.c_str(), (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40, color);
  SDL_BlitSurface(Infos, NULL, Screen, NULL);

  SDL_Rect ligne;
  ligne.x = Infos->w;
  ligne.y = 5;
  ligne.w = 101;
  ligne.h = 1;
  SDL_FillRect(Screen, &ligne, SDL_MapRGB(Screen->format, 255, 0, 0));
  ligne.y = 25;
  SDL_FillRect(Screen, &ligne, SDL_MapRGB(Screen->format, 255, 0, 0));
  ligne.y = 5;
  ligne.w = 1;
  ligne.h = 21;
  SDL_FillRect(Screen, &ligne, SDL_MapRGB(Screen->format, 255, 0, 0));
  ligne.x = Infos->w + 101;
  SDL_FillRect(Screen, &ligne, SDL_MapRGB(Screen->format, 255, 0, 0));
  ligne.x = Infos->w + 1;
  ligne.y = 6;
  ligne.w = (100 * vaisseau->getEnergie()) / vaisseau->getEnergieMax();
  ligne.h = 19;
  SDL_FillRect(Screen, &ligne, SDL_MapRGB(Screen->format, 0, 0, 255));

  SDL_FreeSurface(Infos);
}

void Game::animer(double delay)
{
  level->animer(delay);
  vaisseau->animer(delay);
}

void Game::collisions()
{
  Configuration *configuration = Configuration::getConfiguration();
  std::list<Ennemi*>* ennemis = level->getEnnemList();
  std::list<Projectile*>* projectiles = level->getProjList();
  std::list<Bonus*>* bonus = level->getBonusList();

  // Gère les collisions bonus / vaisseau
  {
    std::list<Bonus*>::iterator it = bonus->begin();
    while((it != bonus->end()) && ((*it)->getPos() < level->getPos()))
    {
      Sprite *bonus = (*it)->getSkin();
      Sprite *vaiss = vaisseau->getSprite();
      if(vaiss->collision(bonus) && vaisseau->getEnergie() > 0)
      {
        // TODO : Gérer l'action du bonus
        // Note : on peut modifier n'importe quel objet : Game, Level, Vaisseau, Ennemis, Bonus, Projectiles ...
        // Il faut développer un "langage" de script permettant de spécifier :
        //  - un objet ou une classe d'objets (le vaisseau, le jeu, le level, tous les ennemis ...)
        //  - une propriété à affecter
        //  - une action sur cette propriété
        //  - des paramètres
        //
        // Exemples : 
        //  - game score incrémenter "nombre de points"
        //  - vaisseau vie positionner maximum
        //  - "ennemis visibles" energie positionner 0
        //
        // Il faut aussi pouvoir préciser plusieurs règles pour pouvoir créer des bonus ayant plusieurs actions.
        //
        executeAction((*it)->getType()->getActions(), (*it)->getType()->getNbActions());
        (*it)->detruire();
      }
      it++;
    }
  }

  // Gère les collisions ennemis / vaisseau
  // TODO : Il serait plus logique que l'ennemi soit juste endommagé, bien que la collision soit alors permanente -> un des deux meurt au bout d'un moment
  // mais lorsque c'est le vaisseau, l'ennemi peut à priori survivre (mur par exemple)
  if(!configuration->spectrum())
  {
    std::list<Ennemi*>::iterator it = ennemis->begin();
    while((it != ennemis->end()) && ((*it)->getPos() < level->getPos()))
    {
      if((*it)->getEnergie())
      {
        Sprite *ennemi = (*it)->getSkin();
        Sprite *vaiss = vaisseau->getSprite();
        if(vaiss->collision(ennemi))
        {
          vaisseau->endommager((*it)->getDegats());
          (*it)->detruire();
        }
      }
      it++;
    }
  }

  // Gère les collisions projectiles / ennemis
  {
    std::list<Ennemi*>::iterator enneIt = ennemis->begin();
    while(enneIt != ennemis->end() && (*enneIt)->getPos() < level->getPos())
    {
      if((*enneIt)->getEnergie())
      {
        Sprite *ennemi = (*enneIt)->getSkin();
        std::list<Projectile*>::iterator projIt = projectiles->begin();
        while(projIt != projectiles->end())
        {
          if(!(*projIt)->getExplose())
          {
            Sprite *proj = (*projIt)->getSkin();
            if(ennemi->collision(proj))
            {
              (*projIt)->detruire();
              score += (*enneIt)->endommager((*projIt)->getDegats());
            }
          }
          projIt++;
        }
      }
      enneIt++;
    }
  }
}

// Ce qui est implémenté pour l'instant :
//  - score : add, sub
//  - lives : add, sub
//  - energy : add, sub, set
void Game::executeAction(BonusAction *actions, int nbActions)
{
  for(int i=0; i < nbActions; i++)
  {
    // On compare l'objet à tous les objets connus, puis la fonction à appliquer à toutes les fonctions connues.
    // Il faudrait trouver un moyen de n'avoir à implémenter qu'une fois chaque méthode, et non pour chaque cible
    // en en faisant abstraction.
    if(actions[i].objet == "score")
    {
      if(actions[i].action == "add")
      {
        if(actions[i].nbParams > 0)
        {
          std::istringstream iss (actions[i].parametres[0]);
          unsigned int bonus;
          iss >> bonus;
          score += bonus;
        }
      }
      if(actions[i].action == "sub")
      {
        if(actions[i].nbParams > 0)
        {
          std::istringstream iss (actions[i].parametres[0]);
          unsigned int malus;
          iss >> malus;
          if(malus >= score)
            score = 0;
          else
            score -= malus;
        }
      }
    }
    if(actions[i].objet == "lives")
    {
      if(actions[i].action == "add")
      {
        if(actions[i].nbParams > 0)
        {
          std::istringstream iss (actions[i].parametres[0]);
          int bonus;
          iss >> bonus;
          vies += bonus;
        }
      }
      if(actions[i].action == "sub")
      {
        if(actions[i].nbParams > 0)
        {
          std::istringstream iss (actions[i].parametres[0]);
          int malus;
          iss >> malus;
          if(malus >= vies)
            vies = 0;
          else
            vies -= malus;
        }
      }
    }
    if(actions[i].objet == "energy")
    {
      if(actions[i].action == "add")
      {
        if(actions[i].nbParams > 0)
        {
          std::istringstream iss (actions[i].parametres[0]);
          unsigned int bonus;
          iss >> bonus;
          vaisseau->reparer(bonus);
        }
      }
      if(actions[i].action == "sub")
      {
        if(actions[i].nbParams > 0)
        {
          std::istringstream iss (actions[i].parametres[0]);
          unsigned int malus;
          iss >> malus;
          vaisseau->endommager(malus);
        }
      }
      if(actions[i].action == "set")
      {
        if(actions[i].nbParams > 0)
        {
          if(actions[i].parametres[0] == "max")
          {
            vaisseau->setEnergie(vaisseau->getEnergieMax());
          }
          else
          {
            std::istringstream iss (actions[i].parametres[0]);
            unsigned int bonus;
            iss >> bonus;
            vaisseau->setEnergie(bonus);
          }
        }
      }
    }
  }
}
