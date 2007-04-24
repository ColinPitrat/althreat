#ifndef _ALTHREAT_VAISSEAU
#define _ALTHREAT_VAISSEAU

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <SDL/SDL.h>
#include "Arme.h"
#include "Projectile.h"
#include "Sprite.h"
#include "graphics.h"

enum Direction { Arret = 0, Gauche = 1, Haut = 2, Droite = 4, Bas = 8 };

class Vaisseau
{
  public:
    Vaisseau();
    ~Vaisseau();

    // Accesseurs
    //SDL_Surface *getPicture();
    Sprite *getSprite() { return skin; };
    bool isDead() { return ((energie == 0) && (destruct->fini())); };
    unsigned int getEnergieMax() { return energieMax; };
    unsigned int getEnergie() { return energie; };
    void setEnergie(unsigned int set) { if(set <= energieMax) energie = set; };
    void positionner(double newx, double newy) { skin->setPos(newx, newy); };
    void setVitesse(double newdx, double newdy) { dx = newdx; dy = newdy; };

    // Méthodes
    void setDirection(unsigned int dir);
    void endommager(unsigned int degats);
    void reparer(unsigned int reparation);

    void afficher();
    void animer(double delay);
    void reset();
    void tirer(std::list<Projectile*>* projectiles);

  private:
    // Propriétés du vaisseau
    double vitesse_max;
    double accel;
    unsigned int energieMax;
    unsigned int nbTypesArmes;
    Arme **armes;
    Sprite *skin;
    Sprite *destruct;

    // Etat actuel du vaisseau
    unsigned int energie;
    // Vitesses
    double dx, dy;
    // Accelerations
    double ddx, ddy;
    bool monostable;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
