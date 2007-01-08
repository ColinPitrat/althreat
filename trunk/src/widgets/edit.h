#ifndef _T3F_SDL_EDIT
#define _T3F_SDL_EDIT

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include "widget.h"
#include "focuscontainer.h"
#include "constantes.h"

class Edit: public Widget
{
  public:
    Edit(SDL_Rect position, std::string *value, Alignement align = T3F_LEFT);
    virtual ~Edit();

    void setFont(std::string fontname, short int fontsize) { font = TTF_OpenFont(fontname.c_str(), fontsize); if(!font) std::cerr << "Error : Loading " << fontname << " : " << TTF_GetError() << std::endl; };
    void setColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { bg = setbg; fg = setfg; border = setborder; };
    void setFocusColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { focusbg = setbg; focusfg = setfg; focusborder = setborder; };
    void setUnactiveColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { unactivebg = setbg; unactivefg = setfg; unactiveborder = setborder; };
    void setBorderSize(unsigned short int size) { borderSize = size; };

    std::string getValue() { return *val; };
    bool getActive() { return !unactive; };
    bool *getUnactivePtr() { return &unactive; };
    void setText(std::string value) { *val = value; };
    void setActive(bool active) { unactive = !active; };
    
    virtual void onFocus(bool foc, bool nextIfUnactive = true);
    virtual void afficher(SDL_Surface *Screen);
    virtual int filtre(const SDL_Event *event);
  private:
    // Style
    TTF_Font *font;
    unsigned short int borderSize;
    SDL_Color border, bg, fg;
    SDL_Color focusborder, focusbg, focusfg;
    SDL_Color unactiveborder, unactivefg, unactivebg;
    bool unactive;

    // Propriétés
    SDL_Rect pos;
    std::string *val;
    Alignement alignement;
};

#endif
