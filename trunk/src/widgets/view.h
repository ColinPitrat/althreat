#ifndef _T3F_SDL_VIEW
#define _T3F_SDL_VIEW

#include "container.h"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class View: public Container
{
  public:
    static View *getView() { return view ? view : new View; };
    ~View();
    virtual int filtre(const SDL_Event *event) { return this->_filtre(event); };
    virtual void afficher(SDL_Surface *Screen) { this->_afficher(Screen); };
    static int _filtre(const SDL_Event *event);
    static void _afficher(SDL_Surface *Screen);
    virtual void onFocus(bool foc, bool nextIfUnactive = true);

  private:
    View();

    static View *view;
};

#endif
