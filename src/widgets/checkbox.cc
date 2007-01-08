#include "checkbox.h"

Checkbox::Checkbox(SDL_Rect position, std::string caption, bool *value)
{
  // Style par défaut
  border = (SDL_Color) {150, 150, 150, 255};
  bg = (SDL_Color) {0, 0, 0, 255};
  fg = (SDL_Color) {255, 0, 0, 255};
  focusborder = (SDL_Color) {150, 150, 150, 255};
  focusbg = (SDL_Color) {0, 0, 0, 255};
  focusfg = (SDL_Color) {255, 255, 0, 255};
  unactiveborder = (SDL_Color) {150, 150, 150, 255};
  unactivebg = (SDL_Color) {0, 0, 0, 255};
  unactivefg = (SDL_Color) {175, 175, 175, 255};
  unactive = false;
  borderSize = 1;
  font = NULL;
  toggleOn = NULL;
  toggleOff = NULL;

  pos.x = position.x; pos.y = position.y; pos.w = position.w; pos.h = position.h;
  val = value;
  label = caption;
  //SDL_Surface *Screen = SDL_GetVideoSurface();
  //bg = setbg;//SDL_MapRGB(Screen->format, setbg.r, setbg.g, setbg.b); 
  //fg = setfg;//SDL_MapRGB(Screen->format, setfg.r, setfg.g, setfg.b);
}

Checkbox::~Checkbox()
{
  if(toggleOn)
  {
    SDL_FreeSurface(toggleOn);
    toggleOn = NULL;
  }
  if(toggleOff)
  {
    SDL_FreeSurface(toggleOff);
    toggleOff = NULL;
  }
  if(font)
  {
    TTF_CloseFont(font);
    font = NULL;
  }
}

void Checkbox::setImages(std::string imageOn, std::string imageOff)
{
  toggleOn = IMG_Load(imageOn.c_str());
  if(toggleOn == NULL)
  {
    std::cerr << "Impossible de charger " << imageOn << " : " << SDL_GetError() << std::endl;
  }
  toggleOff = IMG_Load(imageOff.c_str());
  if(toggleOff == NULL)
  {
    std::cerr << "Impossible de charger " << imageOff << " : " << SDL_GetError() << std::endl;
  }
}

int Checkbox::filtre(const SDL_Event *event)
{
  if(!unactive)
  {
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) 
      if(event->button.x >= pos.x && event->button.x <= (pos.w + pos.x) && event->button.y >= pos.y && event->button.y <= (pos.h + pos.y))
      {
        *val = !*val;
        setFocus(true);
      }
    if (focused && event->type == SDL_KEYDOWN)
      if(event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_SPACE)
        *val = !*val;
  }
  return 1;
}

void Checkbox::afficher(SDL_Surface *Screen)
{
  SDL_Color *pborder, *pbg, *pfg;
  if(unactive)
  {
    pborder = &unactiveborder;
    pbg = &unactivebg;
    pfg = &unactivefg;
  }
  else
  {
    if(focused)
    {
      pborder = &focusborder;
      pbg = &focusbg;
      pfg = &focusfg;
    }
    else
    {
      pborder = &border;
      pbg = &bg;
      pfg = &fg;
    }
  }
  if(font)
  {
    //SDL_Rect foreground;
    SDL_Surface *texte, *image;
    SDL_Rect destRect, srcRect, imgsrcRect, imgdstRect;
    if(borderSize > 0)
    {
      SDL_Rect bord;
      bord.x = pos.x - borderSize; bord.y = pos.y - borderSize; bord.w = pos.w + 2*borderSize; bord.h = pos.h + 2*borderSize;
      SDL_FillRect(Screen, &bord, SDL_MapRGB(Screen->format, pborder->r, pborder->g, pborder->b));
    }
    texte = TTF_RenderText_Shaded(font, label.c_str(), *pfg, *pbg);

    // Positionne l'image
    if(*val)
      image = toggleOn;
    else
      image = toggleOff;

    imgdstRect.x = pos.x; imgdstRect.w = (image->w > pos.w) ? pos.w : image->w; 
    imgdstRect.y = (image->h < pos.h) ? pos.y + (pos.h - image->h) / 2 : pos.y; imgdstRect.h = (image->h < pos.h) ? image->h : pos.h;
    imgsrcRect.x = 0; imgsrcRect.w = (image->w > pos.w) ? pos.w : image->w; 
    imgsrcRect.y = (image->h < pos.h) ? 0 : (image->h - pos.h) / 2; imgsrcRect.h = (image->h < pos.h) ? image->h : pos.h;

    destRect.x = pos.x + imgdstRect.w; destRect.w = texte->w > (pos.w - imgdstRect.w) ? (pos.w - imgdstRect.w) : texte->w; 
    destRect.y = (texte->h < pos.h) ? pos.y + (pos.h - texte->h) / 2 : pos.y; destRect.h = (texte->h < pos.h) ? texte->h : pos.h;
    srcRect.x = 0; srcRect.w = destRect.w; 
    srcRect.y = (texte->h < pos.h) ? 0 : (texte->h - pos.h) / 2; srcRect.h = (texte->h < pos.h) ? texte->h : pos.h;

    SDL_FillRect(Screen, &pos, SDL_MapRGB(Screen->format, pbg->r, pbg->g, pbg->b));
    if(image)
      SDL_BlitSurface(image, &imgsrcRect, Screen, &imgdstRect);
    SDL_BlitSurface(texte, &srcRect, Screen, &destRect);
    SDL_FreeSurface(texte);
  }
}

void Checkbox::onFocus(bool foc, bool nextIfUnactive)
{
  focused = foc;
  if(unactive && foc)
  {
    if(nextIfUnactive)
      container->focusNext();
    else
      container->focusPrev();
  }
}
