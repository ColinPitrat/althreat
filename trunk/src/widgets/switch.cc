#include "switch.h"

Switch::Switch(SDL_Rect position, std::string cap, bool *value)
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

  pos.x = position.x; pos.y = position.y; pos.w = position.w; pos.h = position.h;
  caption = new std::string(cap);
  val = value;
}

Switch::~Switch()
{
  delete caption;
  if(font)
  {
    TTF_CloseFont(font);
    font = NULL;
  }
}

int Switch::filtre(const SDL_Event *event)
{
  if(!unactive)
  {
    if(event->type == SDL_MOUSEMOTION)
    {
      if(event->button.x >= pos.x && event->button.x <=(pos.w + pos.x) && event->button.y >= pos.y && event->button.y <= (pos.h + pos.y))
        setFocus(true);
      else
        setFocus(false);
    }

    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
      if(event->button.x >= pos.x && event->button.x <=(pos.w + pos.x) && event->button.y >= pos.y && event->button.y <= (pos.h + pos.y))
      {
        setFocus(true);
        *val = !*val;
        return 0;
      }
    }

    if(focused && event->type == SDL_KEYDOWN)
    {
      if(event->key.keysym.sym == SDLK_RETURN)
      {
        *val = !*val;
        return 0;
      }
    }
  }

  return 1;
}

void Switch::afficher(SDL_Surface *Screen)
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
    if(borderSize > 0)
    {
      SDL_Rect bord;
      bord.x = pos.x - borderSize; bord.y = pos.y - borderSize; bord.w = pos.w + 2*borderSize; bord.h = pos.h + 2*borderSize;
      SDL_FillRect(Screen, &bord, SDL_MapRGB(Screen->format, pborder->r, pborder->g, pborder->b));
    }
    SDL_FillRect(Screen, &pos, SDL_MapRGB(Screen->format, pbg->r, pbg->g, pbg->b));
    
    SDL_Surface *texte;
    SDL_Rect destRect;
    SDL_Rect srcRect;
    texte = TTF_RenderText_Shaded(font, caption->c_str(), *pfg, *pbg);

    destRect.x = (texte->w < pos.w) ? pos.x + (pos.w - texte->w) / 2 : pos.x; destRect.w = (texte->w < pos.w) ? texte->w : pos.w; 
    destRect.y = (texte->h < pos.h) ? pos.y + (pos.h - texte->h) / 2 : pos.y; destRect.h = (texte->h < pos.h) ? texte->h : pos.h;
    srcRect.x = (texte->w < pos.w) ? 0 : (texte->w - pos.w) / 2; srcRect.w = (texte->w < pos.w) ? texte->w : pos.w; 
    srcRect.y = (texte->h < pos.h) ? 0 : (texte->h - pos.h) / 2; srcRect.h = (texte->h < pos.h) ? texte->h : pos.h;

    SDL_BlitSurface(texte, &srcRect, Screen, &destRect);
    SDL_FreeSurface(texte);
  }
}

void Switch::onFocus(bool foc, bool nextIfUnactive)
{
  int x,y;
  SDL_GetMouseState(&x, &y);
  if(foc && ((x < pos.x) || (x > pos.x + pos.w) || (y < pos.y) || (y > pos.y + pos.h)))
    SDL_WarpMouse(pos.x + pos.w/2, pos.y + pos.h/2); 
  focused = foc;
  if(unactive && foc)
  {
    if(nextIfUnactive)
      container->focusNext();
    else
      container->focusPrev();
  }
}
