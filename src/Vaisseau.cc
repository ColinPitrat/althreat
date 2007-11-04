#include "Vaisseau.h"

Vaisseau::Vaisseau()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : vaisseau" << std::endl;
  SDL_Surface *Screen = SDL_GetVideoSurface();

  std::ifstream fichier;
  std::string liste("vaisseaux/vaisseaux.lst");
  std::string previewFile("preview");
  std::string propFile("proprietes");

  std::string *repertoires;
  std::string *noms;
  Sprite **previews;
  unsigned int nbVaisseaux;

  // Charge les previews, noms et repertoires des vaisseaux
  {
    fichier.open((configuration->getDataDir() + liste).c_str(),std::ios::in);

    fichier >> nbVaisseaux;
    previews = new Sprite*[nbVaisseaux];
    repertoires = new std::string[nbVaisseaux];
    noms = new std::string[nbVaisseaux];
    for(unsigned int i = 0; i < nbVaisseaux; i++)
    {
      std::string pvFile;
      fichier >> noms[i];
      fichier >> repertoires[i];
      pvFile = configuration->getDataDir() + repertoires[i] + previewFile;
      previews[i] = new Sprite(pvFile, configuration->getDataDir());
    }

    fichier.close();
  }

  unsigned int i = 0;
  bool Selection = false;
  // Choix du vaisseau
  do
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == configuration->touche(TOUCHE_ARME))
            Selection = true;
          if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == configuration->touche(TOUCHE_DROITE))
            if(++i == nbVaisseaux)
              i = 0;
          if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == configuration->touche(TOUCHE_GAUCHE))
            if(i == 0)
              i = nbVaisseaux - 1;
            else
              i--;
          break;
        case SDL_JOYAXISMOTION:
          if(configuration->isJoystickEvent(event.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_ARME))
            Selection = true;
          if(configuration->isJoystickEvent(event.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_DROITE))
            if(++i == nbVaisseaux)
              i = 0;
          if(configuration->isJoystickEvent(event.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_GAUCHE))
            if(i == 0)
              i = nbVaisseaux - 1;
            else
              i--;
          break;
        case SDL_JOYBUTTONDOWN:
          if(configuration->isJoystickEvent(event.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, TOUCHE_ARME))
            Selection = true;
          if(configuration->isJoystickEvent(event.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, TOUCHE_DROITE))
            if(++i == nbVaisseaux)
              i = 0;
          if(configuration->isJoystickEvent(event.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, TOUCHE_GAUCHE))
            if(i == 0)
              i = nbVaisseaux - 1;
            else
              i--;
          break;
        case SDL_QUIT:
          SDL_Quit();
          exit(0);
        default:
          break;
      }
    }
    SDL_Surface *Screen = SDL_GetVideoSurface();
    SDL_Surface *Fond = loadImage((configuration->getDataDir() + "images/Choix.png").c_str());
    SDL_BlitSurface(Fond, NULL, Screen, NULL);

    SDL_Color color;
    color.r = 255; color.g = 0; color.b = 0;
    SDL_Surface *nomTxt = Texte(noms[i].c_str(), (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 60, color);
    SDL_Rect dst, dst2;
    dst.x = 250;
    dst.y = 200;
    dst.w = previews[i]->getPicture()->w;
    dst.h = previews[i]->getPicture()->h;
    dst2.x = 500 - nomTxt->w / 2;
    dst2.y = 550 - nomTxt->h / 2;
    dst2.w = nomTxt->w;
    dst2.h = nomTxt->h;
    SDL_BlitSurface(previews[i]->getPicture(), NULL, Screen, &dst);
    SDL_BlitSurface(nomTxt, NULL, Screen, &dst2);

    SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);
    SDL_FreeSurface(Fond);
    SDL_FreeSurface(nomTxt);
  }while(!Selection);

  std::string filename = repertoires[i] + propFile;

  delete[] repertoires;
  delete[] noms;
  for(unsigned int i = 0; i < nbVaisseaux; i++)
    delete previews[i];
  delete[] previews;

  // Chargement du vaisseau

  fichier.open((configuration->getDataDir() + filename).c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;

  std::string skinName;
  std::string destructName;
  std::string currentArmeName;

  fichier >> skinName >> destructName >> vitesse_max >> accel >> energieMax >> nbTypesArmes >> monostable;
  skin = new Sprite(configuration->getDataDir() + skinName, configuration->getDataDir());
  destruct = new Sprite(configuration->getDataDir() + destructName, configuration->getDataDir());
  armes = new Arme*[nbTypesArmes];
  for(unsigned int i = 0; i < nbTypesArmes; i++)
  {
    fichier >> currentArmeName;
    armes[i] = new Arme(currentArmeName);
  }

  if(fichier.fail())
    std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

  fichier.close();

  skin->setPos(0, (Screen->h - skin->getPicture()->h)/ 2);
  SDL_Rect lim;
  lim.x = 0; lim.y = 0; lim.w = Screen->w; lim.h = Screen->h;
  skin->setLimites(&lim);
  dx = 0; dy = 0;
  energie = energieMax;
}

Vaisseau::~Vaisseau()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : vaisseau" << std::endl;
  delete skin;
  delete destruct;
  for(unsigned int i = 0; i < nbTypesArmes; i++)
  {
    delete armes[i];
  }
  delete[] armes;
}

/*SDL_Surface *Vaisseau::getPicture()
  {
// TODO : gérer si le vaisseau est en train d'exploser
return skin->getPicture();
}*/

void Vaisseau::setDirection(unsigned int dir)
{
  if(dir & Gauche)
    ddx = -accel;
  else
  {
    if(dir & Droite)
      ddx = accel;
    else
      ddx = 0;
  }
  if(dir & Haut)
    ddy = -accel;
  else
  {
    if(dir & Bas)
      ddy = accel;
    else
      ddy = 0;
  }
}

void Vaisseau::endommager(unsigned int degats)
{
  if((signed int)energie - (signed int)degats > 0)
  {
    energie -= degats;
  }
  else
  {
    energie = 0;
    const SDL_Rect *pos = skin->getPos();
    destruct->setPos(pos->x, pos->y);
    destruct->reset();
  }
}

void Vaisseau::reparer(unsigned int reparation)
{
  if(energie + reparation < energieMax)
    energie += reparation;
  else
    energie = energieMax;
}

void Vaisseau::afficher()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  if(energie > 0) 
    skin->afficher(Screen);
  else
    destruct->afficher(Screen, false);
}

void Vaisseau::animer(double delay)
{
  if(ddx == 0)
  {
      if(monostable)
      {
        if(dx > 0)
          dx -= delay * accel;
        if(dx < 0)
          dx += delay * accel;
        if(dx > -delay*accel && dx < delay*accel)
          dx = 0;
      }
  }
  else
  {
	  dx += delay * ddx;
	  if(dx > vitesse_max) dx = vitesse_max;
	  if(dx < -vitesse_max) dx = -vitesse_max;
  }
  if(ddy == 0)
  {
      if(monostable)
	  {
		  if(dy > 0)
			  dy -= delay * accel;
		  if(dy < 0)
			  dy += delay * accel;
		  if(dy > -delay*accel && dy < delay*accel)
		    dy = 0;
	  }
  }
  else
  {
	  dy += delay * ddy;
	  if(dy > vitesse_max) dy = vitesse_max;
	  if(dy < -vitesse_max) dy = -vitesse_max;
  }
  if(energie > 0)
  {
    double deltaX = delay * dx;
    double deltaY = delay * dy;
    skin->deplacer(deltaX, deltaY);
    if(skin->isHBlocked())
    {
        dx = 0;
    }
    if(skin->isVBlocked())
    {
        dy = 0;
    }
  }
}

void Vaisseau::tirer(std::list<Projectile*>* projectiles)
{
  for(unsigned int i = 0; i < nbTypesArmes; i++)
  {
    if(armes[i]->estCharge())
    {
      // TODO : Créer un nouveau projectile dans le level
      const SDL_Rect *rect = skin->getPos();
      projectiles->push_back(new Projectile(armes[i], rect->x, rect->y, dx, dy));
      armes[i]->tirer();
    }
  }
}

void Vaisseau::reset()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  skin->setPos(0, (Screen->h - skin->getPicture()->h)/ 2);
  dx = dy = 0;
  energie = energieMax;
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
