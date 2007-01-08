#include <iostream>
#include <string>
#include <unistd.h>
#include "widgets/edit.h"
#include "Game.h"
#include "Menu.h"
#include "MenuJeu.h"
#include "Level.h"
#include "Vaisseau.h"
#include "Configuration.h"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define NB_HIGHSCORES 10 

Game *jeu;
unsigned int level = 0;

// Gère les évenements durant le jeu
static void jeu_events(unsigned int &dir, bool &shoot, bool &FinJeu)
{
  SDL_Event event;
  SDL_Surface *Screen = SDL_GetVideoSurface();
  Configuration *configuration = Configuration::getConfiguration();
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
        if(event.key.keysym.sym == configuration->touche(TOUCHE_HAUT))
          dir |= Haut;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_BAS))
          dir |= Bas;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_GAUCHE))
          dir |= Gauche;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_DROITE))
          dir |= Droite;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_ARME))
          shoot = 1;
        if(event.key.keysym.sym == SDLK_ESCAPE)
        {
          // TODO : Copier l'image du jeu
          SDL_Surface* fondMenu = SDL_CreateRGBSurface(Screen->flags, Screen->w, Screen->h, Screen->format->BitsPerPixel, Screen->format->Rmask, Screen->format->Gmask, Screen->format->Bmask, Screen->format->Amask);
          SDL_BlitSurface(Screen, NULL, fondMenu, NULL);
          MenuJeu *menu = new MenuJeu(fondMenu);
          FinJeu = menu->afficher();
        }
        break;
      case SDL_KEYUP:
        if(event.key.keysym.sym == configuration->touche(TOUCHE_HAUT))
          dir -= Haut;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_BAS))
          dir -= Bas;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_GAUCHE))
          dir -= Gauche;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_DROITE))
          dir -= Droite;
        if(event.key.keysym.sym == configuration->touche(TOUCHE_ARME))
          shoot = 0;
        break;
      case SDL_QUIT:
        FinJeu = true;
        break;
      default:
        break;
    }
  }
}

// Affiche l'aide sur la ligne de commande
void info(std::string appname)
{
  std::cout << "Usage : " << appname << " <options>\n";
  std::cout << "  Options :\n\t-h : Affiche ce message d'aide (help)\n\t-f : Plein écran (fullscreen)\n\t-w : Mode fenêtré (windowed)\n\t-m : Pas de son (mute)\n\t-s : Son activé (sound)\n\t-v : Informations détaillées (verbose)\n\t-d : Informations de débogage (debug)\n\t-p : Mode spectre (phantom)\n";
}

// Traite la fin du jeu (ecran récapitulatif, highscores ...)
void GameOver(unsigned int score, bool practice)
{
  Configuration *configuration = Configuration::getConfiguration();
  bool finished = false;
  SDL_Surface *Screen = SDL_GetVideoSurface();

  std::string infos;
  std::ostringstream oss (std::ostringstream::out);
  oss << "Score : " << score;
  infos = oss.str();

  SDL_Color color; color.r = 180; color.g = 180; color.b = 180;
  SDL_Surface *msg = Texte("Game Over", (configuration->getDataDir() + "fonts/vera.ttf").c_str(), 80, color);
  color.r = 180; color.g = 200; color.b = 30;
  SDL_Surface *inf = Texte(infos.c_str(), (configuration->getDataDir() + "fonts/vera.ttf").c_str(), 40, color);
  SDL_Rect destMsg; destMsg.x = (Screen->w - msg->w) / 2; destMsg.y = (Screen->h - msg->h) / 4; destMsg.w = msg->w; destMsg.h = msg->h;
  SDL_Rect destInf; destInf.x = (Screen->w - inf->w) / 2; destInf.y = 2 * (Screen->h - msg->h) / 4; destInf.w = inf->w; destInf.h = inf->h;

  std::string names[NB_HIGHSCORES];
  std::string name("Votre nom");
  std::string tmpName, tmpNameNext;
  unsigned int tmpScore, tmpScoreNext;
  unsigned int scores[NB_HIGHSCORES];
  memset(scores, 0, sizeof(unsigned int)*NB_HIGHSCORES);
  int pos = -1;
  View *view = NULL;
  FocusContainer *cont = NULL;

  if(!practice)
  {
    std::ifstream fichier;
    fichier.open(configuration->getHighscoresFile().c_str(), std::ios::in);
    if(fichier.is_open())
    {
      for(int i = 0; i < NB_HIGHSCORES; i++)
      {
        fichier >> scores[i];
        if(fichier.eof())
        {
          if(pos == -1)
            pos = i;
          break;
        }
        std::stringbuf nameBuf;
        fichier.get(nameBuf);
        // substr pour enlever l'espace de début
        names[i] = nameBuf.str().substr(1);
        if(score > scores[i] && pos == -1)
          pos = i;
      }
    }
    else
    {
      pos = 0;
    }
    fichier.close();
  }

  if(pos != -1)
  {
    view = View::getView();
    cont = new FocusContainer();
    SDL_Rect text_pos;
    text_pos.x = 250; text_pos.y = 3 * (Screen->h - msg->h) / 4; text_pos.w = Screen->w - 2 * text_pos.x; text_pos.h = 40;
    Edit *text = new Edit(text_pos, &name, T3F_LEFT);
    text->setFont((configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40);
    cont->addWidget(text);
    cont->focus(text);
    view->addWidget(cont);
  }

  while(!finished)
  {
    SDL_BlitSurface(msg, NULL, Screen, &destMsg);
    SDL_BlitSurface(inf, NULL, Screen, &destInf);
    if(pos != -1)
      view->afficher(Screen);
    SDL_Flip(Screen);

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym)
          {
            case SDLK_ESCAPE:
            case SDLK_RETURN:
              finished = true;
              break;
            default:
              break;
          }
          break;
        default:
          break;
      }
    }
  }

  if(pos != -1)
  {
    tmpName = names[pos];
    tmpScore = scores[pos];
    names[pos] = name;
    scores[pos] = score;
    for(int i = pos + 1; i < NB_HIGHSCORES; i++)
    {
      tmpNameNext = names[i];
      tmpScoreNext = scores[i];
      names[i] = tmpName;
      scores[i] = tmpScore;
      tmpName = tmpNameNext;
      tmpScore = tmpScoreNext;
    }

    {
      std::ofstream fichier;
      fichier.open(configuration->getHighscoresFile().c_str(), std::ofstream::trunc | std::ofstream::out);
      for(int i = 0; i < NB_HIGHSCORES; i++)
        if(scores[i])
          fichier << scores[i] << " " << names[i] << std::endl;
      fichier.close();
    }

    if(cont)
    {
      cont->deleteAll();
      delete cont;
    }
  }

  SDL_FreeSurface(msg);
  SDL_FreeSurface(inf);

  if(!practice)
  {
    SDL_FillRect(Screen, NULL, 0);
    SDL_Rect pos1, pos2, pos;
    pos1.x = 100; pos1.w = 450;
    pos2.x = 550; pos2.w = 150;
    pos1.y = 150; pos1.h = 40;
    pos2.y = 150; pos2.h = 40;
    view = View::getView();
    view->clear();
    cont = new FocusContainer();
    pos.x = 100; pos.w = 600;
    pos.y = 50; pos.h = 60;
    Label *lab = new Label(pos, "Hall of Fame", T3F_CENTER);
    lab->setFont((configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 80);
    lab->setColors((SDL_Color) {0, 0, 0, 255}, (SDL_Color) {255, 255, 0, 255}, (SDL_Color) {0, 0, 0, 255});
    lab->setBorderSize(0);
    cont->addWidget(lab);
    for(int i=0; i<NB_HIGHSCORES; i++)
    {
      lab = new Label(pos1, names[i], T3F_LEFT);
      lab->setFont((configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40);
      lab->setBorderSize(0);
      cont->addWidget(lab);
      std::ostringstream oss (std::ostringstream::out);
      oss << scores[i];
      lab = new Label(pos2, oss.str(), T3F_RIGHT);
      lab->setFont((configuration->getDataDir() + "fonts/babelfish.ttf").c_str(), 40);
      lab->setBorderSize(0);
      cont->addWidget(lab);
      pos1.y += 40;
      pos2.y += 40;
    }
    view->addWidget(cont);

    finished = false;
    while(!finished)
    {
      view->afficher(Screen);
      SDL_Flip(Screen);

      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        switch(event.type)
        {
          case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
              case SDLK_ESCAPE:
              case SDLK_RETURN:
                finished = true;
                break;
              default:
                break;
            }
            break;
          default:
            break;
        }
      }
    }

    if(cont)
    {
      cont->deleteAll();
      delete cont;
    }
  }
}

void Jeu(bool practice = false)
{
  bool FinJeu = false;
  bool pause = false;
  unsigned int dir = Arret;
  SDL_Surface *Screen = SDL_GetVideoSurface();
  std::string levelsListName("levels/levels.lst");
  SDL_ShowCursor(SDL_DISABLE);
  Configuration *configuration = Configuration::getConfiguration();

  Uint32 beginning = SDL_GetTicks();
  Uint32 frames = 0;
  bool nextLevel = true;
  while(!FinJeu && !jeu->estMort())
  {
    if(nextLevel)
    {
      FinJeu = !jeu->nextLevel(levelsListName);
      level++;
    }
    bool shoot = false;
    Uint32 debut = SDL_GetTicks();

    while((!FinJeu) && (jeu->getLevel()->getPos() < jeu->getLevel()->getEnd()) && !jeu->getVaisseau()->isDead())
    {
      jeu->animer(((double)SDL_GetTicks() - debut) / 10);
      jeu_events(dir, shoot, FinJeu);

      debut = SDL_GetTicks();
      jeu->collisions();

      jeu->getVaisseau()->setDirection(dir);

      if(shoot)
        jeu->tirer();

      SDL_FillRect(Screen, NULL, 0);  

      jeu->afficher();                    
      SDL_Flip(Screen);                  
      frames++;                           
    }
    if(jeu->getVaisseau()->isDead())
    {
      nextLevel = false;
      jeu->meurt();
    }
    else
      nextLevel = true;
    Uint32 end = SDL_GetTicks();
    if(configuration->verbose())
      std::cout << frames << " images en " << ((double)end - beginning) / 1000 << " secondes -> " << (double)frames * 1000 / ((double)end - beginning) << "FPS\n";
    if(practice && nextLevel)
      break;
  }
  GameOver(jeu->getScore(), practice);

  delete jeu;
  jeu = NULL;
}

int main(int argc, char *argv[])
{
  Configuration *configuration = Configuration::getConfiguration();

  // Traitement des options
  // -m : pas de son
  // -s : avec son
  // -f : plein ecran
  // -w : fenetré
  // -h : affiche l'aide
  // -p : spectrum mode
  // -v : mode verbeux
  {
    char c;
    while ((c = getopt(argc, argv, "mhfwvdps")) != -1)
    {
      switch(c)
      {
        case 'm':
          configuration->setNoSound(true);
          break;
        case 's':
          configuration->setNoSound(false);
          break;
        case 'p':
          configuration->setSpectrum(true);
          break;
        case 'f':
          configuration->setFullscreen(true);
          break;
        case 'w':
          configuration->setFullscreen(false);
          break;
        case 'v':
          configuration->setVerbose(true);
          break;
        case 'd':
          configuration->setDebug(true);
          break;
        case '?':
          std::cerr << "Erreur : Option inconnue `-" << optopt << "'.\n";
          exit(-1);
        case 'h':
        default:
          info(argv[0]);
          exit(0);
      }
    }
  }

  InitSDL();

  bool Quitter = false;
  Menu *menu = Menu::getMenu();

  bool reinit = true;
  while(!Quitter)
  {
    if(reinit)
    {
      menu->init();
    }
    switch(menu->show(jeu))
    {
      case 0:
        Jeu();
        reinit = true;
        break;
      case 1:
        Jeu(true);
        reinit = true;
        break;
        /*case 2:
          reinit = false;
          break;
          case 3:
          reinit = false;
          break;*/
      case 4:
        // Ne devrait pas être necessaire, mais ne fait pas de mal
        if(!configuration->nosound()) Mix_HaltMusic();
        Quitter = true;
        break;
      default:
        std::cerr << "Erreur : choix de menu inconnu.\n";
        reinit = false;
        break;
    }
  }
  delete menu;

  if(!configuration->nosound()) Mix_CloseAudio();
  SDL_Quit();
  delete configuration;
  return 0;
}
