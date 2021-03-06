#include "graphics.h"
#include "lang.h"

void InitSDL()
{
  Configuration *configuration = Configuration::getConfiguration();

  Uint32 sdl_init_flags = SDL_INIT_VIDEO;
  if(!configuration->nosound()) sdl_init_flags |= SDL_INIT_AUDIO;
  if(configuration->joystick()) sdl_init_flags |= SDL_INIT_JOYSTICK;
  if(SDL_Init(sdl_init_flags) < 0)
  {
    std::cerr << "Impossible to initialize SDL: " << SDL_GetError() << std::endl;
    exit(-1);
  }

  SDL_Surface *Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, DEFAULT_BPP, (configuration->fullscreen() ? SDL_FULLSCREEN : 0) | SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_HWACCEL);

  if(Screen == NULL)
  {
    std::cerr << "Impossible to select video mode " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << "x" << DEFAULT_BPP << ": " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(-1);
  }

  if((Screen->flags & SDL_DOUBLEBUF) != SDL_DOUBLEBUF && configuration->verbose())
    std::cout << "Double buffering isn't available." << std::endl;

  if(!configuration->nosound())
  {
    // TODO : Selon la configuration, utiliser 44100 ou 22050 Hz, et un format adapté
    // TODO : Trouver un compromis dépendant de la configuration pour la longueur du buffer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
      std::cerr << "Impossible to open audio channel: " << Mix_GetError() << std::endl;
      configuration->setNoSound(true);
    }
    else
    // Maintenant que le son est initialisé, on peut appliquer la configuration
    // C'est laid, mais on peut pas faire autrement ! (enfin on doit pouvoir mais je vois pas mieux)
      configuration->applySoundConf();
  }

  if(configuration->joystick())
  {
    int numJoysticks = SDL_NumJoysticks();
    configuration->setNbJoysticks(numJoysticks);
    for(int i=0; i < numJoysticks; i++)
    {
      configuration->setJoystick(i, SDL_JoystickOpen(i));
    }
  }

  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  if (TTF_Init() < 0) 
  {
    std::cerr << "Impossible to initialize SDL_TTF: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(-1);
  }

  SDL_WM_SetCaption (_("AlThreat"), NULL);
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) 
  {
    case 1:
      *p = pixel;
      break;
    case 2:
      *(Uint16 *)p = pixel;
      break;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      } 
      else 
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;
    case 4:
      *(Uint32 *)p = pixel;
      break;
  }
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) 
  {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
    default:
      return 0;
  }
}

SDL_Surface *loadImage(const char *fileName)
{
  SDL_Surface *image, *tmp;

  //image = IMG_Load(fileName);
  tmp = IMG_Load(fileName);
  if(tmp == NULL)
  {
    std::cerr << "Impossible to load " << fileName << ": " << SDL_GetError() << std::endl;
    return NULL;
  }

  image = SDL_DisplayFormatAlpha(tmp);
  if(image == NULL)
  {
    std::cerr << "Impossible to convert " << fileName << " to screen format: " << SDL_GetError() << std::endl;
    return tmp;
  }
  SDL_FreeSurface(tmp);

  return image;
}

SDL_Surface *Texte(std::string texte, const char *font_face, short font_size, SDL_Color fgColor)
{
  TTF_Font *font;
  SDL_Surface *text;

  font = TTF_OpenFont(font_face, font_size); // Chargement de la police
  if (!font) 
    std::cerr << "Impossible to load font size" << font_size << "pt from " << font_face << ": " << SDL_GetError() << std::endl;

  text = TTF_RenderText_Blended(font, texte.c_str(), fgColor);

  // Fermeture de la police
  TTF_CloseFont(font); 

  return text;
}

// vim:shiftwidth=2:shiftround:expandtab:cindent
