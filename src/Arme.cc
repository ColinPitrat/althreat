#include "Arme.h"

Arme::Arme(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Construction : arme" << std::endl;
  std::ifstream fichier;
  fichier.open((configuration->getDataDir() + filename).c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;

  std::string sonName;
  std::string sonDestructName;

  fichier >> degats >> x_init >> y_init >> dx_init >> dy_init >> lumique >> loadTime >> skinName >> destructName >> sonName >> sonDestructName;

  if(fichier.fail())
    std::cerr << "Erreur lors de la lecture du fichier " << filename << std::endl;

  son = Mix_LoadWAV((configuration->getDataDir() + sonName).c_str());
  sonDestruct = Mix_LoadWAV((configuration->getDataDir() + sonDestructName).c_str());

  fichier.close();
}

Arme::~Arme()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destruction : arme" << std::endl;
  Mix_FreeChunk(son);
  Mix_FreeChunk(sonDestruct);
}
