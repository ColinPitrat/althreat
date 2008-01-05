#ifndef _ALTHREAT_CONF
#define _ALTHREAT_CONF

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Joystick.h"

#define MAX_JOY_SENSIBILITY 32767
#define DEFAULT_JOY_SENSIBILITY 10000
#define JOY_SENSIBILITY_STEP 1000
#define SIGN(x) ((x)>(signed int)configuration->joySensibility() ? 1 : ((x)<-(signed int)configuration->joySensibility() ? -1 : 0))

// Les touches sont :
//  0 - Haut
//  1 - Bas
//  2 - Gauche
//  3 - Droite
//  4 - Arme
//  5 - Arme spéciale
//  6 - Bouclier
enum Controles { TOUCHE_HAUT = 0, TOUCHE_BAS, TOUCHE_GAUCHE, TOUCHE_DROITE, TOUCHE_ARME, TOUCHE_ARME2, TOUCHE_BOUCLIER, TOUCHE_PAUSE };
static const int nbControles = 8;

class Configuration
{
  public:
    static Configuration *getConfiguration() { return configuration ? configuration : new Configuration(); };
    ~Configuration();

    bool nosound() { return _nosound; };
    bool joystick() { return _joystick; };
    bool spectrum() { return _spectrum; };
    bool fullscreen() { return _fullscreen; };
    bool verbose() { return _verbose; };
    bool debug() { return _debug; };
    int musicvol() { return _musicvol; };
    int soundFXvol() { return _soundFXvol; };
    unsigned int joySensibility() { return _joysens; };
    int touche(Controles touche) { return keys[touche]; };
    std::string language() { return _lang; };
    std::string getDataDir() { return datadir; };
    std::string getHighscoresFile() { return highscoresFile; };

    bool toggleFullscreen() { _fullscreen = !_fullscreen; SDL_WM_ToggleFullScreen(SDL_GetVideoSurface()); return _fullscreen; };
    bool toggleNosound();
    void setMusicVol(unsigned int vol) { _musicvol = vol; if(!_nosound) Mix_VolumeMusic(vol); };
    void setSoundFXVol(unsigned int vol) { _soundFXvol = vol; if(!_nosound) Mix_Volume(-1, vol); };
    void setNoSound(bool opt) { _nosound = opt; };
    void setJoySensibility(unsigned int opt) { _joysens = opt; };
    void setJoystick(bool opt) { _joystick = opt; };
    void setSpectrum(bool opt) { _spectrum = opt; };
    void setFullscreen(bool opt) { _fullscreen = opt; };
    void setVerbose(bool opt) { _verbose = opt; };
    void setDebug(bool opt) { _debug = opt; };
    void setTouche(Controles touche, int keycode) { keys[touche] = keycode; };
    void setNbJoysticks(int opt);
    void setLanguage(std::string opt) { _lang = opt; setlocale(LC_ALL, _lang.c_str()); };
    bool setJoystick(int i, SDL_Joystick *joy);
    void setJoystickEvent(Controles control, int event, int joystick, int but_or_ax, int val) { joybuttons[control].type = event; joybuttons[control].which = joystick; joybuttons[control].button_or_axis = but_or_ax; joybuttons[control].value = val; };
    bool isJoystickEvent(int event, int joystick, int but_or_ax, int val, Controles control);
    void applySoundConf() { Mix_VolumeMusic(_musicvol); Mix_Volume(-1, _soundFXvol); };
    bool save();
  private:
    Configuration();

    static Configuration *configuration;
    int keys[nbControles];
    int _musicvol;
    int _soundFXvol;
    unsigned int _joysens;
    bool _nosound;
    bool _spectrum;
    bool _fullscreen;
    bool _verbose;
    bool _debug;
    bool _joystick;
    std::string _lang;
    std::string datadir;
    std::string confFile;
    std::string highscoresFile;
    int nbJoysticks;
    joystick_event joybuttons[nbControles];
    SDL_Joystick **joysticks;
};

#endif

// vim:shiftwidth=2:shiftround:expandtab:cindent
