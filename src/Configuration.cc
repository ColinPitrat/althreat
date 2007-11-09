#include "Configuration.h"

Configuration *Configuration::configuration = NULL;

Configuration::Configuration()
{
  // Valeures par défaut 
  _nosound = false;
  _spectrum = false;
  _fullscreen = false;
  _joystick = true;
  _musicvol = MIX_MAX_VOLUME;
  _soundFXvol = MIX_MAX_VOLUME;
  _verbose = false;
  _debug = false;
  nbJoysticks = 0;
  joysticks = NULL;

  char* locale = setlocale(LC_ALL, NULL);
  std::cout << "locale : " << locale << std::endl;
  if(locale && strstr(locale, "fr"))
    _lang = "fr_FR";
  else 
    if(locale && strstr(locale, "en"))
      _lang = "en_US";
    else
      _lang = "C";

  // Keyboard default controls
  keys[TOUCHE_HAUT] = SDLK_UP;
  keys[TOUCHE_BAS] = SDLK_DOWN;
  keys[TOUCHE_GAUCHE] = SDLK_LEFT;
  keys[TOUCHE_DROITE] = SDLK_RIGHT;
  keys[TOUCHE_ARME] = SDLK_SPACE;
  keys[TOUCHE_ARME2] = SDLK_f;
  keys[TOUCHE_BOUCLIER] = SDLK_q;
  keys[TOUCHE_PAUSE] = SDLK_ESCAPE;

  // Joystick default controls
  joybuttons[TOUCHE_HAUT].which = 0;
  joybuttons[TOUCHE_BAS].which = 0;
  joybuttons[TOUCHE_GAUCHE].which = 0;
  joybuttons[TOUCHE_DROITE].which = 0;
  joybuttons[TOUCHE_ARME].which = 0;
  joybuttons[TOUCHE_ARME2].which = 0;
  joybuttons[TOUCHE_BOUCLIER].which = 0;
  joybuttons[TOUCHE_PAUSE].which = 0;
  joybuttons[TOUCHE_HAUT].button_or_axis = 0;
  joybuttons[TOUCHE_BAS].button_or_axis = 0;
  joybuttons[TOUCHE_GAUCHE].button_or_axis = 1;
  joybuttons[TOUCHE_DROITE].button_or_axis = 1;
  joybuttons[TOUCHE_ARME].button_or_axis = 0;
  joybuttons[TOUCHE_ARME2].button_or_axis = 1;
  joybuttons[TOUCHE_BOUCLIER].button_or_axis = 2;
  joybuttons[TOUCHE_PAUSE].button_or_axis = 3;
  joybuttons[TOUCHE_HAUT].value = 0;
  joybuttons[TOUCHE_BAS].value = 1;
  joybuttons[TOUCHE_GAUCHE].value = 0;
  joybuttons[TOUCHE_DROITE].value = 1;
  joybuttons[TOUCHE_ARME].value = 1;
  joybuttons[TOUCHE_ARME2].value = 1;
  joybuttons[TOUCHE_BOUCLIER].value = 1;
  joybuttons[TOUCHE_PAUSE].value = 1;

  confFile = (std::string)getenv("HOME") + "/" + ".AlThreat.conf";
  highscoresFile = (std::string)getenv("HOME") + "/" + ".AlThreat.high";

  // Recherche des données
  {
    std::string file("levels/levels.lst");
    std::ifstream fichier;
    fichier.open(((std::string)DATADIR + "/" + file).c_str(), std::ios::in);
    if(!fichier.is_open())
    {
      // Il semblerait qu'ouvrir un fichier 2 fois de suite ne fonctionne pas si la première ouverture échoue !
      std::ifstream fichier;
      fichier.open(((std::string)SRCDIR + "/" + file).c_str(), std::ios::in);
      if(!fichier.is_open())
      {
        std::ifstream fichier;
        fichier.open(("../" + file).c_str(), std::ios::in);
        if(!fichier.is_open())
        {
          std::cerr << "Error: Impossible to find the directory containing data files.\n";
          std::cerr << "Tried pathes: \n";
          std::cerr << " - " << (std::string)DATADIR + "/" << std::endl;
          std::cerr << " - " << (std::string)SRCDIR + "/" << std::endl;
          std::cerr << " - " << "../" << std::endl;
          exit(0);
        }
        else
        {
          datadir = (std::string)"../";
        }
        fichier.close();
      }
      else
      {
        datadir = (std::string)SRCDIR + "/";
      }
      fichier.close();
    }
    else
    {
      datadir = (std::string)DATADIR + "/";
    }
    fichier.close();
  }

  // Chargement de la configuration 
  std::ifstream fichier;
  fichier.open(confFile.c_str(), std::ios::in);
  if(fichier.is_open())
  {
    while(!fichier.eof())
    {
      std::string propertieName;
      fichier >> propertieName;
      if(propertieName=="Mute")
        fichier >> _nosound;
      if(propertieName=="MusicVol")
        fichier >> _musicvol;
      if(propertieName=="FXVol")
        fichier >> _soundFXvol;
      if(propertieName=="Fullscreen")
        fichier >> _fullscreen;
      if(propertieName=="Language")
        fichier >> _lang;
      if(propertieName=="Touche")
      {
        int i;
        fichier >> i;
        fichier >> keys[i];
      }
      if(propertieName=="JoyButton")
      {
        int i;
        unsigned int utmp;
        signed int stmp;
        fichier >> i;
        fichier >> utmp; joybuttons[i].type = utmp;
        fichier >> utmp; joybuttons[i].which = utmp;
        fichier >> utmp; joybuttons[i].button_or_axis = utmp;
        fichier >> stmp; joybuttons[i].value = stmp;
      }
    }
    fichier.close();
  }

  configuration = this;
}

Configuration::~Configuration()
{
  delete[] joysticks;
}

bool Configuration::toggleNosound()
{
  if(nosound())
  {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
      std::cerr << "Impossible to initialize SDL audio module: " << SDL_GetError() << std::endl;
      return true;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
      std::cerr << "Impossible to open audio channel: " << Mix_GetError() << std::endl;
      return true;
    }
    Mix_Volume(-1, _soundFXvol);
    Mix_VolumeMusic(_musicvol);
    _nosound = false;
    return false;
  }
  else
  {
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_CloseAudio();
    _nosound = true;
    return true;
  }
}

void Configuration::setNbJoysticks(int opt)
{ 
  if(opt > 0) 
  { 
    nbJoysticks = opt; 
    joysticks = new SDL_Joystick *[nbJoysticks]; 
    for(int i = 0; i < nbJoysticks; i++)
      joysticks[i] = NULL;
  }
}

bool Configuration::setJoystick(int i, SDL_Joystick *joy)
{ 
  if(i < nbJoysticks && joysticks[i] == NULL) 
  {
    joysticks[i] = joy; 
    return true;
  }
  return false;
}

bool Configuration::isJoystickEvent(int event, int joystick, int but_or_ax, int val, Controles control)
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

bool Configuration::save()
{
  std::ofstream fichier;
  fichier.open(confFile.c_str(), std::ofstream::trunc | std::ofstream::out);

  fichier << "Mute " << _nosound << std::endl;
  fichier << "MusicVol " << _musicvol << std::endl;
  fichier << "FXVol " << _soundFXvol << std::endl;
  fichier << "Fullscreen " << _fullscreen << std::endl;
  fichier << "Language " << _lang << std::endl;
  for(int i = 0; i < nbControles; i++)
    fichier << "Touche " << i << " " << keys[i] << std::endl;
  for(int i = 0; i < nbControles; i++)
    fichier << "JoyButton " << i << " " << (unsigned int) joybuttons[i].type << " " << (unsigned int) joybuttons[i].which << " " << (unsigned int) joybuttons[i].button_or_axis << " " << (signed int) joybuttons[i].value << std::endl;

  fichier.close();

  // Sauvegarder la configuration
  return true;
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
