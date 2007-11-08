#include "TypeBonus.h"

TypeBonus::TypeBonus(std::string filename)
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Constructor: type bonus" << std::endl;
  std::ifstream fichier;
  fichier.open((configuration->getDataDir() + filename).c_str(), std::ios::in);
  if(!fichier)
    std::cerr << "Impossible to open file " << filename << std::endl;

  std::string sonName;
  std::string skinName;

  fichier >> skinName >> sonName >> nbActions;
  actions = new BonusAction[nbActions];
  for(int i=0; i < nbActions; i++)
  {
    fichier >> actions[i].objet >> actions[i].action >> actions[i].nbParams;
    actions[i].parametres = new std::string[actions[i].nbParams];
    for(int j=0; j < actions[i].nbParams; j++)
      fichier >> actions[i].parametres[j];
  }

  if(fichier.fail())
    std::cerr << "Error while reading reading file " << filename << std::endl;

  skin = new SpriteData(configuration->getDataDir() + skinName, configuration->getDataDir());

  son = Mix_LoadWAV((configuration->getDataDir() + sonName).c_str());

  fichier.close();
}

TypeBonus::~TypeBonus()
{
  Configuration *configuration = Configuration::getConfiguration();
  if(configuration->debug())
    std::cerr << "Destructor: type bonus" << std::endl;
  delete skin;
  Mix_FreeChunk(son);
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
