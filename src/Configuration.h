#ifndef _ALTHREAT_CONF
#define _ALTHREAT_CONF

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Les touches sont :
//  0 - Haut
//  1 - Bas
//  2 - Gauche
//  3 - Droite
//  4 - Arme
//  5 - Arme spéciale
//  6 - Bouclier
enum Controles { TOUCHE_HAUT = 0, TOUCHE_BAS, TOUCHE_GAUCHE, TOUCHE_DROITE, TOUCHE_ARME, TOUCHE_ARME2, TOUCHE_BOUCLIER };
static const int nbControles = 7;

class Configuration
{
  public:
    static Configuration *getConfiguration() { return configuration ? configuration : new Configuration(); };
    ~Configuration();

    bool nosound() { return _nosound; };
    bool spectrum() { return _spectrum; };
    bool fullscreen() { return _fullscreen; };
    bool verbose() { return _verbose; };
    bool debug() { return _debug; };
    int musicvol() { return _musicvol; };
    int soundFXvol() { return _soundFXvol; };
    int touche(Controles touche) { return keys[touche]; };
    std::string getDataDir() { return datadir; };
    std::string getHighscoresFile() { return highscoresFile; };

    bool toggleFullscreen() { _fullscreen = !_fullscreen; SDL_WM_ToggleFullScreen(SDL_GetVideoSurface()); return _fullscreen; };
    bool toggleNosound();
    void setMusicVol(unsigned int vol) { _musicvol = vol; if(!_nosound) Mix_VolumeMusic(vol); };
    void setSoundFXVol(unsigned int vol) { _soundFXvol = vol; if(!_nosound) Mix_Volume(-1, vol); };
    void setNoSound(bool opt) { _nosound = opt; };
    void setSpectrum(bool opt) { _spectrum = opt; };
    void setFullscreen(bool opt) { _fullscreen = opt; };
    void setVerbose(bool opt) { _verbose = opt; };
    void setDebug(bool opt) { _debug = opt; };
    void setTouche(Controles touche, int keycode) { keys[touche] = keycode; }
    void applySoundConf() { Mix_VolumeMusic(_musicvol); Mix_Volume(-1, _soundFXvol); };
    bool save();
  private:
    Configuration();

    static Configuration *configuration;
    int keys[nbControles];
    int _musicvol;
    int _soundFXvol;
    bool _nosound;
    bool _spectrum;
    bool _fullscreen;
    bool _verbose;
    bool _debug;
    std::string datadir;
    std::string confFile;
    std::string highscoresFile;
};

#endif
