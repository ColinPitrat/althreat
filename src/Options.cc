#include "Options.h"

Options::Options(SDL_Surface *setfond, Mix_Music *setmusique, std::string setmusic_file)
{
  configuration = Configuration::getConfiguration();
  fond = setfond;
  musique = setmusique;
  music_file = setmusic_file;
  optionsLayer = new FocusContainer();

  SDL_Rect fs_pos;
  fs_pos.x = 70; fs_pos.y = 150; fs_pos.w = 300; fs_pos.h = 25;
  std::string textefs = "Plein ecran";
  Checkbox *fs_box = new Checkbox(fs_pos, textefs, &fs_var);
  fs_box->setImages((configuration->getDataDir() + "images/on.png").c_str(), (configuration->getDataDir() + "images/off.png").c_str());
  fs_box->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  fs_box->setBorderSize(0);

  SDL_Rect ns_pos;
  ns_pos.x = 70; ns_pos.y = 180; ns_pos.w = 300; ns_pos.h = 25;
  std::string textens = "Son muet";
  Checkbox *ns_box = new Checkbox(ns_pos, textens, &ns_var);
  ns_box->setImages((configuration->getDataDir() + "images/on.png").c_str(), (configuration->getDataDir() + "images/off.png").c_str());
  ns_box->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  ns_box->setBorderSize(0);

  SDL_Rect mus_label_pos;
  mus_label_pos.x = 70; mus_label_pos.y = 230; mus_label_pos.w = 250; mus_label_pos.h = 25;
  Label *mus_label = new Label(mus_label_pos, "Volume musique :", T3F_LEFT);
  mus_label->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  mus_label->setBorderSize(0);

  SDL_Rect mus_pos;
  mus_pos.x = 330; mus_pos.y = 230; mus_pos.w = 300; mus_pos.h = 25;
  Jauge *mus_jauge = new Jauge(mus_pos, 0, MIX_MAX_VOLUME, &mus_vol, T3F_HOR);

  SDL_Rect fx_label_pos;
  fx_label_pos.x = 70; fx_label_pos.y = 265; fx_label_pos.w = 250; fx_label_pos.h = 25;
  Label *fx_label = new Label(fx_label_pos, "Volume effets sonores :", T3F_LEFT);
  fx_label->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  fx_label->setBorderSize(0);

  SDL_Rect fx_pos;
  fx_pos.x = 330; fx_pos.y = 265; fx_pos.w = 300; fx_pos.h = 25;
  Jauge *fx_jauge = new Jauge(fx_pos, 0, MIX_MAX_VOLUME, &fx_vol, T3F_HOR);

  SDL_Rect control_pos;
  control_pos.x = 70; control_pos.y = 330; control_pos.w = 300; control_pos.h = 25;
  Switch *control_bouton = new Switch(control_pos, "Modifier les controles", &controlModif);
  control_bouton->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);

  SDL_Rect ok_pos;
  ok_pos.x = 100; ok_pos.y = 530; ok_pos.w = 80; ok_pos.h = 25;
  Switch *ok_bouton = new Switch(ok_pos, "OK", &ok);
  ok_bouton->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);

  SDL_Rect cancel_pos;
  cancel_pos.x = 250; cancel_pos.y = 530; cancel_pos.w = 80; cancel_pos.h = 25;
  Switch *cancel_bouton = new Switch(cancel_pos, "Cancel", &cancel);
  cancel_bouton->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);

  if(configuration->nosound())
  {
    mus_label->setActive(false);
    mus_jauge->setActive(false);
    fx_label->setActive(false);
    fx_jauge->setActive(false);
  }

  unactiveMusLabel = mus_label->getUnactivePtr();
  unactiveMusJauge = mus_jauge->getUnactivePtr();
  unactiveFXLabel = fx_label->getUnactivePtr();
  unactiveFXJauge = fx_jauge->getUnactivePtr();

  optionsLayer->addWidget(fs_box);
  optionsLayer->addWidget(ns_box);
  optionsLayer->addWidget(mus_label);
  optionsLayer->addWidget(mus_jauge);
  optionsLayer->addWidget(fx_label);
  optionsLayer->addWidget(fx_jauge);
  optionsLayer->addWidget(control_bouton);
  optionsLayer->addWidget(ok_bouton);
  optionsLayer->addWidget(cancel_bouton);
  optionsLayer->focus(ok_bouton);
}

Options::~Options()
{
  if(optionsLayer)
  {
    optionsLayer->deleteAll();
    delete optionsLayer;
  }
}

void Options::events()
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
            cancel = true;
            break;
          default:
            break;
        }
        break;
      case SDL_QUIT:
        cancel = true;
        break;
      default:
        break;
    }
  }
}

void Options::afficher()
{
  // TODO : Qualité (réduire la qualité des sons, des textes  ...)
  SDL_Surface *Screen = SDL_GetVideoSurface();
  View *view = View::getView();
  ok = false;
  cancel = false;
  controlModif = false;
  fs_var = configuration->fullscreen();
  ns_var = configuration->nosound();
  mus_vol = configuration->musicvol();
  fx_vol = configuration->soundFXvol();
  keysModified = false;

  view->clear();
  view->addWidget(optionsLayer);

  while(!(ok || cancel))
  {
    SDL_BlitSurface(fond, NULL, Screen, NULL);
    view->afficher(Screen);
    SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);
    events();
    if(ns_var && !(*unactiveMusLabel))
    {
      (*unactiveMusLabel) = true;
      (*unactiveMusJauge) = true;
      (*unactiveFXLabel) = true;
      (*unactiveFXJauge) = true;
    }
    if(!ns_var && (*unactiveMusLabel))
    {
      (*unactiveMusLabel) = false;
      (*unactiveMusJauge) = false;
      (*unactiveFXLabel) = false;
      (*unactiveFXJauge) = false;
    }

    // Modification des touches
    if(controlModif)
    {
      keysModified = true;
      FocusContainer *controlsLayer = NULL;
      view->clear();
      SDL_Rect item_pos;
      item_pos.x = 100; item_pos.w = 150; item_pos.h = 25;
      std::string actions[nbControles] = { "Haut", "Bas", "Gauche", "Droite", "Feu", "Arme spéciale", "Bouclier" };

      controlsLayer = new FocusContainer();
      view->addWidget(controlsLayer);
      Label *item;
      for(int i=0; i < nbControles; i++)
      {
        item_pos.y = 150 + i*50; 
        item = new Label(item_pos, actions[i], T3F_LEFT);
        item->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
        item->setBorderSize(0);
        controlsLayer->addWidget(item);
        SDL_BlitSurface(fond, NULL, Screen, NULL);
        view->afficher(Screen);
        SDL_UpdateRect(Screen, 0, 0, Screen->w, Screen->h);
        bool choisi = false;
        while(!choisi)
        {
          SDL_Event event;
          while(SDL_PollEvent(&event))
          {
            switch(event.type)
            {
              case SDL_KEYDOWN:
                keys[i] = event.key.keysym.sym;
                choisi = true;
                break;
              default:
                break;
            }
          }
        }
      }
      view->clear();
      view->addWidget(optionsLayer);
      controlsLayer->deleteAll();
      delete controlsLayer;
      controlModif = false;
    }
  }
  if(ok)
  {
    if(configuration->fullscreen() != fs_var)
      configuration->toggleFullscreen();
    if(configuration->nosound() != ns_var)
    {
      if(!configuration->toggleNosound())
      {
        if(!musique && music_file != "")
          musique = Mix_LoadMUS(music_file.c_str());
        if(!musique)
          std::cerr << "Erreur : Impossible de charger la musique : " << music_file << std::endl;
        else
          if(Mix_FadeInMusic(musique, -1, 1000) == -1)
            std::cerr << "Erreur : Impossible de jouer la musique : " << Mix_GetError() << std::endl;
      }
    }
    if(mus_vol != configuration->musicvol())
    {
      configuration->setMusicVol(mus_vol);
      if(!configuration->nosound())
      {
        Mix_HaltMusic();
        if(!musique)
          std::cerr << "Erreur : Impossible de charger la musique : " << music_file << std::endl;
        else
          if(Mix_FadeInMusic(musique, -1, 1000) == -1)
            std::cerr << "Erreur : Impossible de jouer la musique du menu : " << Mix_GetError() << std::endl;
      }
    }
    if(fx_vol != configuration->soundFXvol())
    {
      configuration->setSoundFXVol(fx_vol);
    }
    if(keysModified)
      for(int i = 0; i < nbControles; i++)
        configuration->setTouche((Controles)i, keys[i]);
    configuration->save();
  }
}
