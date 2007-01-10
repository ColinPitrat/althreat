#ifndef _T3F_SDL_CHECKBOX
#define _T3F_SDL_CHECKBOX

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "widget.h"
#include "container.h"
#include "focuscontainer.h"

class Checkbox: public Widget
{
  public:
    Checkbox(SDL_Rect position, std::string caption, bool *value);
    virtual ~Checkbox();

    void setFont(std::string fontname, short int fontsize) { font = TTF_OpenFont(fontname.c_str(), fontsize); if(!font) std::cerr << "Error : Loading " << fontname << " : " << TTF_GetError() << std::endl; };
    void setColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { bg = setbg; fg = setfg; border = setborder; };
    void setFocusColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { focusbg = setbg; focusfg = setfg; focusborder = setborder; };
    void setUnactiveColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder) { unactivebg = setbg; unactivefg = setfg; unactiveborder = setborder; };
    void setBorderSize(unsigned short int size) { borderSize = size; };
    void setImages(std::string imageOn, std::string imageOff);
    bool isFocusable() { return !unactive; }

    bool getValue() { return *val; };
    bool getActive() { return !unactive; };
    bool *getUnactivePtr() { return &unactive; };
    void setValue(bool value) { *val = value; };
    void setActive(bool active) { unactive = !active; };

    virtual void onFocus(bool foc, bool nextIfUnactive = true);
    virtual void afficher(SDL_Surface *Screen);
    virtual int filtre(const SDL_Event *event);

  private:
    // Style
    TTF_Font *font;
    SDL_Surface *toggleOn;
    SDL_Surface *toggleOff;
    SDL_Color border, fg, bg;
    SDL_Color focusborder, focusfg, focusbg;
    SDL_Color unactiveborder, unactivefg, unactivebg;
    bool unactive;
    unsigned short int borderSize;

    // Propriétés
    std::string label;
    bool *val;
    SDL_Rect pos;
};

#endif
