#ifndef _ALTHREAT_PROJECTILE
#define _ALTHREAT_PROJECTILE

#include "Arme.h"
#include "Sprite.h"
#include "Configuration.h"

class Projectile
{
public:
  Projectile(Arme *settype, double initx, double inity, double initdx, double initdy);
  ~Projectile();

  bool horsEcran();
  bool isDead() { return (explose && destruct->fini()); };
  void animer(double delay) { if(!explose) { double deltaX = delay * dx; double deltaY = delay * dy; skin->deplacer(deltaX, deltaY); }};
  void afficher();
  void detruire();
  unsigned int getDegats() { return type->getDegats(); };
  bool getExplose() { return explose; };
  Arme *getType() { return type; };
  Sprite *getSkin() { return skin; };

private:
  Arme *type;
  Sprite *skin;
  Sprite *destruct;
  bool explose;
  double dx;
  double dy;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
