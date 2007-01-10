#ifndef _T3F_SDL_MENUBUTTON
#define _T3F_SDL_MENUBUTTON

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include "widget.h"
#include "focuscontainer.h"

class MenuButton: public Widget
{
  public:
    MenuButton(SDL_Rect position, std::string cap, short int setValue, short int *value, bool *press);
    virtual ~MenuButton();

    void setFont(std::string fontname, short int fontsize) { font = TTF_OpenFont(fontname.c_str(), fontsize); if(!font) std::cerr << "Error : Loading " << fontname << " : " << TTF_GetError() << std::endl; };
    void setColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { bg = setbg; fg = setfg; border = setborder; };
    void setFocusColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { focusbg = setbg; focusfg = setfg; focusborder = setborder; };
    void setUnactiveColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { unactivebg = setbg; unactivefg = setfg; unactiveborder = setborder; };
    void setBorderSize(unsigned short int size) { borderSize = size; };
    SDL_Rect *getPos() { return &pos; };
    bool isFocusable() { return !unactive; }

    void setActive(bool active) { unactive = !active; };
    bool getActive() { return !unactive; };
    bool *getUnactivePtr() { return &unactive; };
    virtual void onFocus(bool foc, bool nextIfUnactive = true);
    void setCaption(std::string cap) { *caption = cap; };

    virtual void afficher(SDL_Surface *Screen);
    virtual int filtre(const SDL_Event *event);
  private:
    // Style
    TTF_Font *font;
    SDL_Color border, bg, fg;
    SDL_Color focusborder, focusbg, focusfg;
    SDL_Color unactiveborder, unactivefg, unactivebg;
    bool unactive;
    unsigned short int borderSize;

    // Propri�t�s
    bool *pressed;
    short int *val;
    short int setVal;
    SDL_Rect pos;
    std::string* caption;
};

#endif
