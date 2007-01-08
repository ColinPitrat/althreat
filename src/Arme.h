#ifndef _ALTHREAT_ARME
#define _ALTHREAT_ARME

#include <fstream>
#include <iostream>
#include <string>
#include <SDL/SDL_mixer.h>
#include "Sprite.h"
#include "Configuration.h"

class Arme
{
public:
  Arme(std::string filename);
  ~Arme();

  // Accesseurs
  void tirer() { lastShoot = SDL_GetTicks(); };
  bool estCharge() { return (lastShoot + loadTime <= SDL_GetTicks()); };
  bool isLumique() { return lumique; };
  unsigned int getX() { return x_init; };
  unsigned int getY() { return y_init; };
  double getDx() { return dx_init; };
  double getDy() { return dy_init; };
  unsigned int getDegats() { return degats; };
  std::string getSkin() { return skinName; };
  std::string getDestructSkin() { return destructName; };
  Mix_Chunk *getSon() { return son; };
  Mix_Chunk *getSonDestruct() { return sonDestruct; };

  // Méthodes

private:
  unsigned int degats;
  unsigned int x_init, y_init;
  double dx_init, dy_init;
  bool lumique;
  Uint32 lastShoot;
  unsigned int loadTime;
  std::string skinName;
  std::string destructName;
  Mix_Chunk *son;
  Mix_Chunk *sonDestruct;
};

#endif
