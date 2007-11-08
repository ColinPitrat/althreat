#include "Arme.h"

Arme::Arme(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Constructor: arme" << std::endl;
  std::ifstream fichier;
  fichier.open((configuration->getDataDir() + filename).c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Impossible to open file " << filename << std::endl;

  std::string sonName;
  std::string sonDestructName;

  fichier >> degats >> x_init >> y_init >> dx_init >> dy_init >> lumique >> loadTime >> skinName >> destructName >> sonName >> sonDestructName;

  if(fichier.fail())
    std::cerr << "Error while reading the file " << filename << std::endl;

  son = Mix_LoadWAV((configuration->getDataDir() + sonName).c_str());
  sonDestruct = Mix_LoadWAV((configuration->getDataDir() + sonDestructName).c_str());

  fichier.close();
}

Arme::~Arme()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destructor: arme" << std::endl;
  Mix_FreeChunk(son);
  Mix_FreeChunk(sonDestruct);
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
