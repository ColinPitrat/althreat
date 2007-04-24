#ifndef _ALTHREAT_BONUS
#define _ALTHREAT_BONUS

#include "TypeBonus.h"
#include "Configuration.h"

class Bonus
{
public:
  Bonus(TypeBonus *settype, int setpos, double y, double dx, double dy);
  ~Bonus();

  int getPos() { return pos; };
  void afficher();
  void animer(double delay) { double deltaX = ((double)delay) * dx; double deltaY = ((double)delay) * dy; skin->deplacer(deltaX, deltaY); };
  Sprite *getSkin() { return skin; };
  bool horsEcran();
  bool isDead() { return dead; };
  void detruire();
  TypeBonus *getType() { return type; };

private:
  TypeBonus *type;
  Sprite *skin;
  int pos;
  bool dead;
  double x;
  double y;
  double dx;
  double dy;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
