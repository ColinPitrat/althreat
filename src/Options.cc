#include "Options.h"
#include "lang.h"

Options::Options(SDL_Surface *setfond, Mix_Music *setmusique, std::string setmusic_file)
{
  configuration = Configuration::getConfiguration();
  fond = setfond;
  musique = setmusique;
  music_file = setmusic_file;
  optionsLayer = new FocusContainer();

  SDL_Rect fs_pos;
  fs_pos.x = 70; fs_pos.y = 150; fs_pos.w = 300; fs_pos.h = 25;
  std::string textefs = _("Fullscreen");
  Checkbox *fs_box = new Checkbox(fs_pos, textefs, &fs_var);
  fs_box->setImages((configuration->getDataDir() + "images/on.png").c_str(), (configuration->getDataDir() + "images/off.png").c_str());
  fs_box->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  fs_box->setBorderSize(0);

  SDL_Rect ns_pos;
  ns_pos.x = 70; ns_pos.y = 180; ns_pos.w = 300; ns_pos.h = 25;
  std::string textens = _("Mute");
  Checkbox *ns_box = new Checkbox(ns_pos, textens, &ns_var);
  ns_box->setImages((configuration->getDataDir() + "images/on.png").c_str(), (configuration->getDataDir() + "images/off.png").c_str());
  ns_box->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  ns_box->setBorderSize(0);

  SDL_Rect mus_label_pos;
  mus_label_pos.x = 70; mus_label_pos.y = 230; mus_label_pos.w = 250; mus_label_pos.h = 25;
  Label *mus_label = new Label(mus_label_pos, _("Music volume:"), T3F_LEFT);
  mus_label->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  mus_label->setBorderSize(0);

  SDL_Rect mus_pos;
  mus_pos.x = 330; mus_pos.y = 230; mus_pos.w = 300; mus_pos.h = 25;
  Jauge *mus_jauge = new Jauge(mus_pos, 0, MIX_MAX_VOLUME, &mus_vol, T3F_HOR);

  SDL_Rect fx_label_pos;
  fx_label_pos.x = 70; fx_label_pos.y = 265; fx_label_pos.w = 250; fx_label_pos.h = 25;
  Label *fx_label = new Label(fx_label_pos, _("Sounds FX volume:"), T3F_LEFT);
  fx_label->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  fx_label->setBorderSize(0);

  SDL_Rect fx_pos;
  fx_pos.x = 330; fx_pos.y = 265; fx_pos.w = 300; fx_pos.h = 25;
  Jauge *fx_jauge = new Jauge(fx_pos, 0, MIX_MAX_VOLUME, &fx_vol, T3F_HOR);

  SDL_Rect control_pos;
  control_pos.x = 70; control_pos.y = 330; control_pos.w = 300; control_pos.h = 25;
  Switch *control_bouton = new Switch(control_pos, _("Modify controls"), &controlModif);
  control_bouton->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);

  SDL_Rect lang_label_pos;
  lang_label_pos.x = 70; lang_label_pos.y = 365; lang_label_pos.w = 250; lang_label_pos.h = 25;
  Label *lang_label = new Label(lang_label_pos, _("Language:"), T3F_LEFT);
  lang_label->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);
  lang_label->setBorderSize(0);

  SDL_Rect lang_pos;
  lang_pos.x = 330; lang_pos.y = 365; lang_pos.w = 300; lang_pos.h = 25;
  lang_list = new Liste(lang_pos);
  lang_list->setFont((configuration->getDataDir() + "fonts/babelfish.ttf"), 30);
  for(int i=0; i < sizeof(language_names)/sizeof(std::string); i++)
    lang_list->addValue(new std::string(language_names[i]));

  SDL_Rect ok_pos;
  ok_pos.x = 100; ok_pos.y = 530; ok_pos.w = 80; ok_pos.h = 25;
  Switch *ok_bouton = new Switch(ok_pos, _("OK"), &ok);
  ok_bouton->setFont((configuration->getDataDir() + "fonts/vera.ttf").c_str(), 20);

  SDL_Rect cancel_pos;
  cancel_pos.x = 250; cancel_pos.y = 530; cancel_pos.w = 80; cancel_pos.h = 25;
  Switch *cancel_bouton = new Switch(cancel_pos, _("Cancel"), &cancel);
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
  optionsLayer->addWidget(lang_label);
  optionsLayer->addWidget(lang_list);
  optionsLayer->addWidget(ok_bouton);
  optionsLayer->addWidget(cancel_bouton);
  optionsLayer->focus(ok_bouton);

  for(int i=0; i < nbControles; i++)
    keys[i] = joybuttons[i].type = 0;
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
      case SDL_JOYAXISMOTION:
        if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_HAUT))
          optionsLayer->focusPrev();
        if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_BAS))
          optionsLayer->focusNext();
        {
          // For other events we build an event corresponding to return key down and send it to optionsLayer->filtre
          SDL_Event ev;
          ev.type = SDL_KEYDOWN;
          ev.key.state = SDL_PRESSED;
          if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_GAUCHE))
          {
            ev.key.keysym.sym = SDLK_LEFT;
            optionsLayer->filtre(&ev);
          }
          if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_DROITE))
          {
            ev.key.keysym.sym = SDLK_RIGHT;
            optionsLayer->filtre(&ev);
          }
          if(configuration->isJoystickEvent(event.jaxis.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, TOUCHE_ARME))
          {
            ev.key.keysym.sym = SDLK_RETURN;
            optionsLayer->filtre(&ev);
          }
        }
        break;
      case SDL_JOYBUTTONDOWN:
        if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, 1, TOUCHE_HAUT))
          optionsLayer->focusPrev();
        if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, 1, TOUCHE_BAS))
          optionsLayer->focusNext();
        {
          // For other events we build an event corresponding to return key down and send it to optionsLayer->filtre
          SDL_Event ev;
          ev.type = SDL_KEYDOWN;
          ev.key.state = SDL_PRESSED;
          if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, 1, TOUCHE_GAUCHE))
          {
            ev.key.keysym.sym = SDLK_LEFT;
            optionsLayer->filtre(&ev);
          }
          if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, 1, TOUCHE_DROITE))
          {
            ev.key.keysym.sym = SDLK_RIGHT;
            optionsLayer->filtre(&ev);
          }
          if(configuration->isJoystickEvent(event.jbutton.type, event.jbutton.which, event.jbutton.button, 1, TOUCHE_ARME))
          {
            ev.key.keysym.sym = SDLK_RETURN;
            optionsLayer->filtre(&ev);
          }
        }
        break;
      case SDL_QUIT:
        SDL_Quit();
        exit(0);
      default:
        break;
    }
  }
}

bool Options::isJoystickEvent(int event, int joystick, int but_or_ax, int val, Controles control)
{
  if(event == SDL_JOYAXISMOTION)
    return joybuttons[control].type == SDL_JOYAXISMOTION &&
           joystick == joybuttons[control].which && 
           but_or_ax == joybuttons[control].button_or_axis && 
           SIGN(val) == SIGN(joybuttons[control].value);
  else
    return (joybuttons[control].type == SDL_JOYBUTTONUP || joybuttons[control].type == SDL_JOYBUTTONDOWN) &&
           joystick == joybuttons[control].which && 
           but_or_ax == joybuttons[control].button_or_axis && 
           val == joybuttons[control].value; 
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
      FocusContainer *controlsLayer = NULL;
      view->clear();
      SDL_Rect item_pos;
      item_pos.x = 100; item_pos.w = 150; item_pos.h = 25;
      std::string actions[nbControles] = { N_("Up"), N_("Down"), N_("Left"), N_("Right"), N_("Fire"), N_("Special weapon"), N_("Shield"), N_("Pause") };

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
          // To ignore repetitions or mistake (same keys for two actions)
          bool ignore = false;
          while(SDL_PollEvent(&event))
          {
            switch(event.type)
            {
              case SDL_KEYDOWN:
                for(int j=0; j < i; j++)
                  if(keys[j] == event.key.keysym.sym)
                    ignore=true;
                if(!ignore)
                {
                  keys[i] = event.key.keysym.sym;
                  choisi = true;
                  keysModified = true;
                }
                break;
              case SDL_JOYAXISMOTION:
                if(SIGN(event.jaxis.value) != 0)
                {
                  for(int j=0; j < i; j++)
                    if(isJoystickEvent(event.type, event.jaxis.which, event.jaxis.axis, event.jaxis.value, (Controles) j))
                      ignore=true;
                  if(!ignore)
                  {
                    joybuttons[i].type = event.type;
                    joybuttons[i].which = event.jaxis.which;
                    joybuttons[i].button_or_axis = event.jaxis.axis;
                    joybuttons[i].value = event.jaxis.value;
                    choisi = true;
                    joysModified = true;
                  }
                }
                break;
              case SDL_JOYBUTTONDOWN:
                for(int j=0; j < i; j++)
                  if(isJoystickEvent(event.type, event.jbutton.which, event.jbutton.button, event.jbutton.state, (Controles) j))
                    ignore=true;
                if(!ignore)
                {
                  joybuttons[i].type = event.type;
                  joybuttons[i].which = event.jbutton.which;
                  joybuttons[i].button_or_axis = event.jbutton.button;
                  joybuttons[i].value = event.jbutton.state;
                  choisi = true;
                  joysModified = true;
                }
                break;
              case SDL_QUIT:
                SDL_Quit();
                exit(0);
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
          std::cerr << "Error: Impossible to load music: " << music_file << std::endl;
        else
          if(Mix_FadeInMusic(musique, -1, 1000) == -1)
            std::cerr << "Error: Impossible to load music: " << Mix_GetError() << std::endl;
      }
    }
    if(mus_vol != configuration->musicvol())
    {
      configuration->setMusicVol(mus_vol);
      if(!configuration->nosound())
      {
        Mix_HaltMusic();
        if(!musique && music_file != "")
          musique = Mix_LoadMUS(music_file.c_str());
        if(!musique)
          std::cerr << "Error: Impossible to load music: " << music_file << std::endl;
        else
          if(Mix_FadeInMusic(musique, -1, 1000) == -1)
            std::cerr << "Error: Impossible to load menu music: " << Mix_GetError() << std::endl;
      }
    }
    if(fx_vol != configuration->soundFXvol())
    {
      configuration->setSoundFXVol(fx_vol);
    }
    std::string lang_var = language_codes[lang_list->getIntValue()];
    if(lang_var != configuration->language())
      configuration->setLanguage(lang_var);
    if(keysModified || joysModified)
      for(int i = 0; i < nbControles; i++)
      {
        if(keys[i] != 0) 
          configuration->setTouche((Controles)i, keys[i]);
        if(joybuttons[i].type != 0)
          configuration->setJoystickEvent((Controles)i, joybuttons[i].type, joybuttons[i].which, joybuttons[i].button_or_axis, joybuttons[i].value);
      }
    configuration->save();
  }
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
