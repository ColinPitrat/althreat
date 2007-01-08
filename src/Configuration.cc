#include "Configuration.h"

Configuration *Configuration::configuration = NULL;

Configuration::Configuration()
{
  // Valeures par défaut 
  _nosound = false;
  _spectrum = false;
  _fullscreen = false;
  _musicvol = MIX_MAX_VOLUME;
  _soundFXvol = MIX_MAX_VOLUME;
  _verbose = false;
  _debug = false;
  keys[TOUCHE_HAUT] = SDLK_UP;
  keys[TOUCHE_BAS] = SDLK_DOWN;
  keys[TOUCHE_GAUCHE] = SDLK_LEFT;
  keys[TOUCHE_DROITE] = SDLK_RIGHT;
  keys[TOUCHE_ARME] = SDLK_SPACE;
  keys[TOUCHE_ARME2] = SDLK_f;
  keys[TOUCHE_BOUCLIER] = SDLK_q;

  confFile = (std::string)getenv("HOME") + "/" + ".AlThreat.conf";
  highscoresFile = (std::string)getenv("HOME") + "/" + ".AlThreat.high";

  // Rechcerche des données
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
          std::cerr << "Erreur : Impossible de déterminer le répertoire contenant les fichiers de données.\n";
          std::cerr << "Chemins essayés : \n";
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
      if(propertieName=="Touche")
      {
        int i;
        fichier >> i;
        fichier >> keys[i];
      }
    }
    fichier.close();
  }

  configuration = this;
}

Configuration::~Configuration()
{
}

bool Configuration::toggleNosound()
{
  if(nosound())
  {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
      std::cerr << "Impossible d'initialiser SDL audio : " << SDL_GetError() << std::endl;
      return true;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
      std::cerr << "Impossible d'ouvrir le canal audio : " << Mix_GetError() << std::endl;
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

bool Configuration::save()
{
  std::ofstream fichier;
  fichier.open(confFile.c_str(), std::ofstream::trunc | std::ofstream::out);

  fichier << "Mute " << _nosound << std::endl;
  fichier << "MusicVol " << _musicvol << std::endl;
  fichier << "FXVol " << _soundFXvol << std::endl;
  fichier << "Fullscreen " << _fullscreen << std::endl;
  for(int i = 0; i < nbControles; i++)
    fichier << "Touche " << i << " " << keys[i] << std::endl;

  fichier.close();

  // Sauvegarder la configuration
  return true;
}
