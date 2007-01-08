#ifndef _STARWARS_MENU
#define _STARWARS_MENU

#include <SDL/SDL.h>
#include "Options.h"
#include "Game.h"
#include "Sprite.h"
#include "widgets/view.h"
#include "widgets/focuscontainer.h"
#include "widgets/switch.h"
#include "widgets/menubutton.h"

class Menu
{
  public:
    // Pour pouvoir utiliser les boutons, il faut des fonctions statiques
    // On limitera donc l'utilisation � un seul menu !
    static Menu *getMenu() { return (menu) ? menu : menu = new Menu(); };
    unsigned char show(Game *&jeu);
    ~Menu();
    void init();

  private:
    Menu();

    void affichage_menu();
    void menu_events();

    void show_options();

    void show_credits();
    void affichage_credits();
    void credits_events();

    // Credits
    bool FinCredits;
    
    // Apparence
    SDL_Surface *fond;
    Mix_Music *musique;

    // Choix
    signed short int nbChoix;
    signed short int Selected;
    char **texte;
    bool choisi;

    // Autres
    FocusContainer *menuLayer;
    FocusContainer *creditsLayer;
    static Menu *menu;
    Configuration *configuration;
};

#endif
