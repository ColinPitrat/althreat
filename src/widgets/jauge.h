#ifndef _T3F_SDL_JAUGE
#define _T3F_SDL_JAUGE

#include <SDL/SDL.h>
#include "widget.h"
#include "container.h"
#include "focuscontainer.h"
#include "constantes.h"

class Jauge: public Widget
{
  public:
    Jauge(SDL_Rect position, int setmin, int setmax, int *value, Orientation orient);
    virtual ~Jauge() { };

    void setColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder);
    void setFocusColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder);
    void setUnactiveColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder);
    void setBorderSize(unsigned short int size) { borderSize = size; };

    void setActive(bool active) { unactive = !active; };
    bool getActive() { return !unactive; };
    bool *getUnactivePtr() { return &unactive; };
    int getValue() { return *val; };
    void setValue(int value) { *val = value; };

    virtual void onFocus(bool foc, bool nextIfUnactive = true);
    virtual void afficher(SDL_Surface *Screen);
    virtual int filtre(const SDL_Event *event);

  private:
    // Style
    Uint32 border, bg, fg;
    Uint32 focusborder, focusbg, focusfg;
    Uint32 unactiveborder, unactivefg, unactivebg;
    bool unactive;
    unsigned short int borderSize;

    // Propriétés
    bool inuse;
    SDL_Rect pos;
    Orientation orientation;
    int min;
    int max;
    int *val;
};

#endif
