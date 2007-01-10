#ifndef _T3F_SDL_WIDGET
#define _T3F_SDL_WIDGET

#include "SDL/SDL.h"

// On a besoin de savoir que la classe FocusContainer existe pour résoudre les dépendances
class FocusContainer;

class Widget
{
public:
  Widget() { focused = false; container = NULL; };
  virtual ~Widget() {};

  virtual void afficher(SDL_Surface *Screen) = 0;
  virtual int filtre(const SDL_Event *event) = 0;

  void setFocus(bool foc);
  void setContainer(FocusContainer *c) { container = c; };
  virtual bool isFocusable() = 0;

  // Place le flag focused. Ne doit pas être appelée directement
  virtual void onFocus(bool foc, bool nextIfUnactive = true) = 0;

protected:
  FocusContainer *container;
  bool focused;
};

#endif
