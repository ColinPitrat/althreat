#ifndef _STARWARS_MENUJEU
#define _STARWARS_MENUJEU

#include <SDL/SDL.h>
#include "Options.h"
#include "Configuration.h"
#include "widgets/view.h"
#include "widgets/focuscontainer.h"
#include "widgets/menubutton.h"

class MenuJeu
{
  public:
    MenuJeu(SDL_Surface *fond, std::string music_file);
    ~MenuJeu();

    bool afficher();

    void afficher_menu();
    void events();
    void show_options();

  private:
    signed short int nbChoix;
    signed short int Selected;
    bool choisi;
    std::string *texte;

    Configuration *configuration;
    FocusContainer *menuLayer;
    SDL_Surface *fond;
    std::string music_file;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
