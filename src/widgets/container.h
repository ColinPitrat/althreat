#ifndef _T3F_SDL_CONTAINER
#define _T3F_SDL_CONTAINER

#include "widget.h"
#include <list>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

typedef std::list<Widget*> typeliste;

class Container: public Widget
{
  public:
    Container();
    ~Container();
    virtual int filtre(const SDL_Event *event);
    virtual void onFocus(bool foc, bool nextIfUnactive = true);
    virtual void afficher(SDL_Surface *Screen);
    virtual void addWidget(Widget *widget) { liste.push_back(widget); };
    virtual void clear() { liste.clear(); };
    virtual void deleteAll();
    virtual bool containFocusableWidget();
    virtual bool isFocusable() { return containFocusableWidget(); }

  protected:
    typeliste liste;
};

#endif
