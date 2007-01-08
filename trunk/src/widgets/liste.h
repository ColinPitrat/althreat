#ifndef _T3F_SDL_LISTE
#define _T3F_SDL_LISTE

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include "widget.h"
#include "focuscontainer.h"

class Liste: public Widget
{
  public:
    Liste(SDL_Rect position);
    virtual ~Liste();

    void setFont(std::string fontname, short int fontsize) { font = TTF_OpenFont(fontname.c_str(), fontsize); if(!font) std::cerr << "Error : Loading " << fontname << " : " << TTF_GetError() << std::endl; };
    void setColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder, SDL_Color setselect) { bg = setbg; fg = setfg; border = setborder; select = setselect; };
    void setFocusColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder, SDL_Color setselect) { focusbg = setbg; focusfg = setfg; focusborder = setborder; focusselect = setselect; };
    void setUnactiveColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder, SDL_Color setselect) { unactivebg = setbg; unactivefg = setfg; unactiveborder = setborder; unactiveselect = setselect; };
    void setBorderSize(unsigned short int size) { borderSize = size; };
    void setSlideSize(unsigned short int size) { slideSize = size; };

    int getIntValue();
    std::string getValue() { return val ? *val : ""; };
    void addValue(std::string *value) { choix.push_back(value); nbVals++; first = choix.begin(); };
    void delValue(std::string *value) { choix.remove(value); nbVals--; first = choix.begin(); };
    void setActive(bool active) { unactive = !active; };
    bool getActive() { return !unactive; };
    bool *getUnactivePtr() { return &unactive; };
    void nextValue();
    void prevValue();
    // TODO : setValue
    // void setValue(std::string value) { *val = value; };

    virtual void onFocus(bool foc, bool nextIfUnactive = true);
    virtual void afficher(SDL_Surface *Screen);
    virtual int filtre(const SDL_Event *event);
  private:
    // Style
    TTF_Font *font;
    unsigned short int borderSize;
    unsigned short int slideSize;
    SDL_Color border, bg, fg, select;
    SDL_Color focusborder, focusbg, focusfg, focusselect;
    SDL_Color unactiveborder, unactivefg, unactivebg, unactiveselect;
    bool unactive;

    // Propriétés
    int nbVals, numFirst;
    SDL_Rect pos;
    std::list<std::string*> choix;
    std::string *val;
    std::list<std::string*>::iterator first; // Iterateur sur le premier élément visible
};

#endif
