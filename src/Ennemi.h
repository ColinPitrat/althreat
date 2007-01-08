#ifndef _ALTHREAT_ENNEMI
#define _ALTHREAT_ENNEMI

#include "TypeEnnemi.h"
#include "Configuration.h"

class Ennemi
{
public:
  Ennemi(TypeEnnemi *settype, int setpos, double y, double dx, double dy);
  ~Ennemi();

  int getPos() { return pos; };
  void afficher();
  void animer(double delay);
  Sprite *getSkin() { return skin; };
  bool horsEcran();
  bool isDead() { return (!energie && destruct->fini()); };
  unsigned int getEnergie() { return energie; };
  unsigned int getDegats() { return type->getDegats(); };
  unsigned int getValeur() { return type->getValeur(); };
  unsigned int endommager(unsigned int degats);
  void detruire();
  TypeEnnemi *getType() { return type; };

private:
  TypeEnnemi *type;
  Sprite *skin;
  Sprite *destruct;
  int pos;
  unsigned int energie;
  double x;
  double y;
  double dx;
  double dy;
};

#endif
