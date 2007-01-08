#ifndef _ALTHREAT_LEVEL
#define _ALTHREAT_LEVEL

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include "TypeEnnemi.h"
#include "Ennemi.h"
#include "Bonus.h"
#include "Projectile.h"
#include "graphics.h"

#define NB_ETOILES 500
#define ETOILES_VIT_MIN 10
#define ETOILES_VIT_MAX 35
#define ETOILES_VIT_RATIO 100.0

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

struct stars
{
  double x,y;                   // Position de l'étoile 
  double dx;                    // Vitesse de l'étoile
  Uint32 color;                 // Couleur de l'étoile
};

class Level
{
public:
  Level(std::string filename);
  ~Level();

  double getPos() { return pos; };
  unsigned int getEnd() { return longueur; };
  std::list<Projectile*>* getProjList() { return &projectiles; };
  std::list<Ennemi*>* getEnnemList() { return &ennemis; };
  std::list<Bonus*>* getBonusList() { return &bonus; };

  void afficher();
  void animer(double delay);
  void playMusic();
  void stopMusic();

private:
  unsigned int longueur;
  double pos;
  Mix_Music *musique; 
  unsigned int nbTypesEnnemis;
  unsigned int nbTypesBonus;
  TypeEnnemi **typesEnnemis;
  TypeBonus **typesBonus;
  std::list<Ennemi*> ennemis;
  std::list<Projectile*> projectiles;
  std::list<Bonus*> bonus;
  stars etoiles[NB_ETOILES];
};

#endif
