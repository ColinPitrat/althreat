#include "Menu.h"

Menu *Menu::menu = NULL;

Menu::Menu()
{
  musique = NULL;
  menuLayer = NULL;
  creditsLayer = NULL;
  texte = NULL;
  fond = NULL;

  SDL_Surface *Screen = SDL_GetVideoSurface();
  configuration = Configuration::getConfiguration();
  if(!configuration->nosound())
  {
    musique = Mix_LoadMUS((configuration->getDataDir() + "sons/Menusong.ogg").c_str());
    if(!musique)
      std::cerr << "Erreur lors du chargement de la musique du menu : " << Mix_GetError() << std::endl;

  }
  fond = loadImage((configuration->getDataDir() + "images/Menu.png").c_str());
  Selected = 0;

  nbChoix = 5;
  texte = new std::string[nbChoix];
  texte[0] = "Aventure";
  texte[1] = "Entrainement";
  texte[2] = "Options";
  texte[3] = "Credits";
  texte[4] = "Quitter";

  SDL_Rect dest[nbChoix];
  MenuButton *choixBoutons[nbChoix];
  menuLayer = new FocusContainer();
  for(int i = 0; i < nbChoix; i++)
  {
    dest[i].w = 180; 
    dest[i].h = 40;
    dest[i].x = (Screen->w - dest[i].w) / 2;
    dest[i].y = (Screen->h - nbChoix * dest[i].h - (nbChoix - 1) * dest[i].h / 2) / 2 + i * dest[i].h + (i - 1) * dest[i].h;
    choixBoutons[i] = new MenuButton(dest[i], texte[i], i, &Selected, &choisi);
    choixBoutons[i]->setFont((configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 30);
    menuLayer->addWidget(choixBoutons[i]);
  }
  menuLayer->focus(choixBoutons[0]);
}

Menu::~Menu()
{
  if(musique)
  {
    Mix_HaltMusic();
    Mix_FreeMusic(musique);
  }
  if(fond)
  {
    SDL_FreeSurface(fond);
  }
  if(texte)
  {
    delete[] texte;
  }
  if(menuLayer)
  {
    // TODO : Il serait plus logique que la destruction du conteneur détruise le contenu
    // Si on veut préserver le contenu, on fait un clear
    menuLayer->deleteAll();
    delete menuLayer;
  }
  if(creditsLayer)
  {
    creditsLayer->deleteAll();
    delete creditsLayer;
  }
}

void Menu::init()
{
  SDL_ShowCursor(SDL_ENABLE);

  if(musique && (Mix_FadeInMusic(musique, -1, 1000) == -1))
  {
    std::cerr << "Erreur : Impossible de jouer la musique du menu : " << Mix_GetError() << std::endl;
  }
}

void Menu::show_options()
{
  Options *options = new Options(fond, musique, configuration->getDataDir() + "sons/Menusong.ogg");
  options->afficher();
}

void Menu::show_credits()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  FinCredits = false;

  if(!creditsLayer)
  {
    creditsLayer = new FocusContainer();

    SDL_Rect ok_pos;
    ok_pos.x = 360; ok_pos.y = 525; ok_pos.w = 80; ok_pos.h = 25;
    Switch *ok_bouton = new Switch(ok_pos, "OK", &FinCredits);
    ok_bouton->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);

    creditsLayer->addWidget(ok_bouton);
    creditsLayer->focus(ok_bouton);
  }

  View *view;
  view = View::getView();
  view->clear();

  view->addWidget(creditsLayer);
  while(!FinCredits)
  {
    affichage_credits();
    view->afficher(Screen);
    SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);
    credits_events();
  }
}

void Menu::affichage_credits()
{
  SDL_Surface *Screen = SDL_GetVideoSurface();

  SDL_Color blue = { 0, 255, 255, 0};
  SDL_Color yellow = { 255, 255, 0, 0 };
  SDL_Color red = { 255, 0, 0, 0 };

  SDL_Surface *text1 = Texte("Programmation : Skippy", (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40, blue);
  SDL_Surface *text2 = Texte("Graphismes : Raz & Skippy", (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40, blue);
  SDL_Surface *text3 = Texte("Musique : Raz & Vavrek", (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40, blue);
  SDL_Surface *text4 = Texte("-=( the3fold )=-", (configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40, yellow);
  SDL_Surface *text5 = Texte("http://www.vavrek.com", (configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20, red);
  SDL_Surface *text6 = Texte("http://the3fold.free.fr", (configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20, red);

  SDL_Rect rect1 = { 70, 200, text1->w, text1->h };
  SDL_Rect rect2, rect3, rect4;
  SDL_Rect rect5 = { 90, 420, text5->w, text5->h };
  SDL_Rect rect6 = { 90, 470, text6->w, text6->h };
  //rect1.x = 70; rect1.y = 200; rect1.w = text1->w; rect1.h = text1->h;
  rect2.x = 70; rect2.y = 250; rect2.w = text2->w; rect2.h = text2->h;
  rect3.x = 70; rect3.y = 300; rect3.w = text3->w; rect3.h = text3->h;
  rect4.x = 500; rect4.y = 450; rect4.w = text4->w; rect4.h = text4->h;

  SDL_BlitSurface(fond, NULL, Screen, NULL);
  SDL_BlitSurface(text1, NULL, Screen, &rect1);
  SDL_BlitSurface(text2, NULL, Screen, &rect2);
  SDL_BlitSurface(text3, NULL, Screen, &rect3);
  SDL_BlitSurface(text4, NULL, Screen, &rect4);
  SDL_BlitSurface(text5, NULL, Screen, &rect5);
  SDL_BlitSurface(text6, NULL, Screen, &rect6);
}

void Menu::credits_events()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
          case SDLK_RETURN:
            FinCredits = true;
            break;
          default:
            break;
        }
        break;
      case SDL_JOYAXISMOTION:
        if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_ARME))
          FinCredits = true;
        break;
      case SDL_JOYBUTTONDOWN:
        if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, 1, TOUCHE_ARME))
          FinCredits = true;
        break;
      case SDL_QUIT:
        SDL_Quit();
        exit(0);
      default:
        break;
    }
  }
}

void Menu::affichage_menu()
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

void Menu::menu_events()
{
  Configuration *conf = Configuration::getConfiguration();
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
        {
          // Quitter doit être le dernier choix
          Selected = nbChoix - 1;
          choisi = true;
        }
        if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == conf->touche(TOUCHE_HAUT))
        {
          if(--Selected < 0)
            Selected += nbChoix;
          menuLayer->focusPrev();
        }
        if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == conf->touche(TOUCHE_BAS))
        {
          if(++Selected >= nbChoix)
            Selected -= nbChoix;
          menuLayer->focusNext();
        }
        if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == conf->touche(TOUCHE_ARME))
          choisi = true;
        break;
      case SDL_JOYAXISMOTION:
        if(SIGN(event.jaxis.value) != 0)
        {
          if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_HAUT))
          {
            if(--Selected < 0)
              Selected += nbChoix;
            menuLayer->focusPrev();
          }
          if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_BAS))
          {
            if(++Selected >= nbChoix)
              Selected -= nbChoix;
            menuLayer->focusNext();
          }
          if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_ARME))
            choisi = true;
        }
        break;
      case SDL_JOYBUTTONDOWN:
        if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, TOUCHE_HAUT))
        {
          if(--Selected < 0)
            Selected += nbChoix;
          menuLayer->focusPrev();
        }
        if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, TOUCHE_BAS))
        {
          if(++Selected >= nbChoix)
            Selected -= nbChoix;
          menuLayer->focusNext();
        }
        if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, TOUCHE_ARME))
          choisi = true;
        break;
      case SDL_QUIT:
        SDL_Quit();
        exit(0);
      default:
        break;
    }
  }
}

unsigned char Menu::show(Game *&jeu)
{
  choisi = false;

  View *view = View::getView();
  SDL_Surface *Screen = SDL_GetVideoSurface();

  while(!choisi)
  {
    view->clear();
    view->addWidget(menuLayer);

    while(!choisi)
    {
      SDL_BlitSurface(fond, NULL, Screen, NULL);
      view->afficher(Screen);
      SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);

      menu_events();
    }

    view->clear();

    switch(Selected)
    {
      case 0:
        jeu = new Game();
        break;
      case 1:
        jeu = new Game(true);
        break;
      case 2:
        show_options();
        choisi = false;
        break;
      case 3:
        show_credits();
        choisi = false;
        break;
      case 4:
      default:
        break;
    }
  }

  return Selected;
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
