#include "TypeEnnemi.h"

TypeEnnemi::TypeEnnemi(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : type ennemi" << std::endl;
  std::ifstream fichier;
  fichier.open((configuration->getDataDir() + filename).c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;

  std::string sonName;
  std::string skinName;
  std::string destructName;

  fichier >> energieMax >> score >> degats >> skinName >> destructName >> sonName;
  if(fichier.fail())
    std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

  skin = new SpriteData(configuration->getDataDir() + skinName, configuration->getDataDir());
  destructSkin = new SpriteData(configuration->getDataDir() + destructName, configuration->getDataDir());

  son = Mix_LoadWAV((configuration->getDataDir() + sonName).c_str());

  fichier.close();
}

TypeEnnemi::~TypeEnnemi()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : type ennemi" << std::endl;
  delete skin;
  delete destructSkin;
  Mix_FreeChunk(son);
}
