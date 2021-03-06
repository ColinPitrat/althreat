#ifndef _STARWARS_OPTIONS
#define _STARWARS_OPTIONS

#include <SDL/SDL.h>
#include "Configuration.h"
#include "graphics.h"
#include "widgets/view.h"
#include "widgets/focuscontainer.h"
#include "widgets/switch.h"
#include "widgets/jauge.h"
#include "widgets/label.h"
#include "widgets/checkbox.h"
#include "widgets/liste.h"
#include "Joystick.h"

class Options
{
  public:
    Options(SDL_Surface *fond, Mix_Music *musique = NULL, std::string setmusic_file = "");
    ~Options();
    void afficher();
    void events();

  private:
    bool isJoystickEvent(int event, int joystick, int but_or_ax, int val, Controles control);

    bool ok;
    bool cancel;
    bool controlModif;
    FocusContainer *optionsLayer;
    Liste *lang_list;
    Configuration *configuration;
    Mix_Music *musique;

    // Options
    bool fs_var;
    bool ns_var;
    int mus_vol;
    int fx_vol;
    int joy_sens;
    int keys[nbControles];
    bool keysModified;

    joystick_event joybuttons[nbControles];
    SDL_Surface *fond;
    bool joysModified;
    bool *unactiveMusLabel;
    bool *unactiveMusJauge;
    bool *unactiveFXLabel;
    bool *unactiveFXJauge;
    bool *unactiveSensLabel;
    bool *unactiveSensJauge;
    std::string music_file;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
