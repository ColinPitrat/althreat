#include "MenuJeu.h"

MenuJeu::MenuJeu(SDL_Surface *setfond)
{
  configuration = Configuration::getConfiguration();
  fond = setfond;

  nbChoix = 3;
  texte = new char*[nbChoix];
  texte[0] = "Retour au jeu";
  texte[1] = "Options";
  texte[2] = "Quitter le jeu";

  SDL_Surface *Screen = SDL_GetVideoSurface();
  SDL_Rect dest[nbChoix];
  MenuButton *choixBoutons[nbChoix];
  menuLayer = new FocusContainer();
  for(int i = 0; i < nbChoix; i++)
  {
    dest[i].w = 200; 
    dest[i].h = 40;
    dest[i].x = (Screen->w - dest[i].w) / 2;
    dest[i].y = (Screen->h - nbChoix * dest[i].h - (nbChoix - 1) * dest[i].h / 2) / 2 + i * dest[i].h + (i - 1) * dest[i].h;
    choixBoutons[i] = new MenuButton(dest[i], texte[i], i, &Selected, &choisi);
    choixBoutons[i]->setFont((configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 30);
    menuLayer->addWidget(choixBoutons[i]);
  }
  menuLayer->focus(choixBoutons[0]);
}

MenuJeu::~MenuJeu()
{
  if(texte)
  {
    delete[] texte;
  }
  if(menuLayer)
  {
    menuLayer->deleteAll();
    delete menuLayer;
  }
}

void MenuJeu::show_options()
{
  // TODO : Passer la musique courante aux options : est-ce possible, est-ce nécessaire ?
  Options *options = new Options(fond);
  options->afficher();
}

void MenuJeu::events()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
        {
          // Retour au jeu doit être le premier choix
          Selected = 0;
          choisi = true;
        }
        if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == configuration->touche(TOUCHE_HAUT))
        {
          if(--Selected < 0)
            Selected += nbChoix;
          menuLayer->focusPrev();
        }
        if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == configuration->touche(TOUCHE_BAS))
        {
          if(++Selected >= nbChoix)
            Selected -= nbChoix;
          menuLayer->focusNext();
        }
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

void MenuJeu::afficher_menu()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  SDL_BlitSurface(fond, NULL, Screen, NULL);

  SDL_Color color;
  color.r = 255; color.g = 255; color.b = 0;

  SDL_Surface *options[nbChoix];
  for(int i=0; i<nbChoix; i++)
  {
    if(Selected == i)
      color.r = 0;
    else
      color.r = 255;
    options[i] = Texte(texte[i], (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 50, color);
  }

  SDL_Rect dest[nbChoix];
  for(int i = 0; i < nbChoix; i++)
  {
    dest[i].x = (Screen->w - options[i]->w) / 2;
    dest[i].y = (Screen->h - nbChoix * options[i]->h - (nbChoix - 1) * options[i]->h / 2) / 2 + i * options[i]->h + (i - 1) * options[i]->h;
    dest[i].w = options[i]->w; 
    dest[i].h = options[i]->h;
  }

  for(int i=0; i<nbChoix; i++)
  {
    SDL_BlitSurface(options[i], NULL, Screen, &dest[i]);
  }

  SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);
}

bool MenuJeu::afficher()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  View *view = View::getView();

  while(true)
  {
    choisi = false;
    view->clear();
    view->addWidget(menuLayer);
    while(!choisi)
    {
      SDL_BlitSurface(fond, NULL, Screen, NULL);
      view->afficher(Screen);
      SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);

      events();
    }

    view->clear();

    Options *options;
    switch(Selected)
    {
      case 2:
        // Quitter le jeu
        return true;
        break;
      case 1:
        // Options
        options = new Options(fond);
        options->afficher();
        break;
      case 0:
      default:
        // Fin de la pause
        return false;
        break;
    }
  }
}
