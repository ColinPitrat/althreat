#ifndef _ALTHREAT_GAME
#define _ALTHREAT_GAME

#include <sstream>
#include "widgets/view.h"
#include "widgets/liste.h"
#include "widgets/focuscontainer.h"
#include "Level.h"
#include "Vaisseau.h"
#include "graphics.h"
#include "Configuration.h"

#define NB_VIES 2

class Game
{
public:
  // Constructeur/Destructeur
  Game();
  Game(bool practice);
  ~Game();

  // Accesseurs
  bool nextLevel(std::string);
  bool resetLevel(std::string);
  Level *getLevel() { return level; };
  Vaisseau *getVaisseau() { return vaisseau; };
  unsigned int getScore() { return score; };
  void afficher();
  void animer(double delay);
  void tirer() { vaisseau->tirer(level->getProjList()); };
  void collisions();
  void executeAction(BonusAction *actions, int nbActions);

  // Methodes
  void meurt() { if(vies > 0) { vies--; vaisseau->reset(); resetLevel("levels/levels.lst"); } };
  bool estMort() { return vies <= 0 && vaisseau->isDead(); };

private:
  Level *level;
  Vaisseau *vaisseau;
  int vies;
  unsigned int numLevel;
  unsigned int score;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
