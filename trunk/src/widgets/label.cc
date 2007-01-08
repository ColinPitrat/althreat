#include "label.h"

Label::Label(SDL_Rect position, std::string value, Alignement align)
{
  // Style par défaut
  border = (SDL_Color) {150, 150, 150, 255};
  bg = (SDL_Color) {0, 0, 0, 255};
  fg = (SDL_Color) {255, 0, 0, 255};
  unactiveborder = (SDL_Color) {150, 150, 150, 255};
  unactivebg = (SDL_Color) {0, 0, 0, 255};
  unactivefg = (SDL_Color) {175, 175, 175, 255};
  unactive = false;
  borderSize = 1;
  font = NULL;

  pos.x = position.x; pos.y = position.y; pos.w = position.w; pos.h = position.h;
  val = value;
  alignement = align;
}

Label::~Label()
{
  if(font)
  {
    TTF_CloseFont(font);
    font = NULL;
  }
}

int Label::filtre(const SDL_Event *)
{
  return 1;
}

void Label::afficher(SDL_Surface *Screen)
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
    pborder = &border;
    pbg = &bg;
    pfg = &fg;
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
    
    SDL_Surface *texte = NULL;
    SDL_Rect destRect;
    SDL_Rect srcRect;
    if(val.length())
    {
      texte = TTF_RenderText_Shaded(font, val.c_str(), *pfg, *pbg);
      switch(alignement)
      {
        case T3F_RIGHT:
          destRect.x = texte->w > pos.w ? pos.x : pos.x + pos.w - texte->w; destRect.w = texte->w > pos.w ? pos.w : texte->w; 
          destRect.y = (texte->h < pos.h) ? pos.y + (pos.h - texte->h) / 2: pos.y; destRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          srcRect.x = texte->w > pos.w ? texte->w - pos.w : 0; srcRect.w = texte->w > pos.w ? pos.w : texte->w; 
          srcRect.y = (texte->h < pos.h) ? 0 : (texte->h -pos.h) / 2; srcRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          break;
        case T3F_CENTER:
          destRect.x = texte->w > pos.w ? pos.x : pos.x + (pos.w - texte->w) / 2; destRect.w = texte->w > pos.w ? pos.w : texte->w; 
          destRect.y = (texte->h < pos.h) ? pos.y + (pos.h - texte->h) / 2: pos.y; destRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          srcRect.x = texte->w > pos.w ? (texte->w - pos.w) / 2 : 0; srcRect.w = texte->w > pos.w ? pos.w : texte->w; 
          srcRect.y = (texte->h < pos.h) ? 0 : (texte->h -pos.h) / 2; srcRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          break;
        case T3F_LEFT:
        default:
          destRect.x = pos.x; destRect.w = texte->w > pos.w ? pos.w : texte->w; 
          destRect.y = (texte->h < pos.h) ? pos.y + (pos.h - texte->h) / 2: pos.y; destRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          srcRect.x = 0; srcRect.w = texte->w > pos.w ? pos.w : texte->w; 
          srcRect.y = (texte->h < pos.h) ? 0 : (texte->h -pos.h) / 2; srcRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          break;
      }
      SDL_BlitSurface(texte, &srcRect, Screen, &destRect);
    }
    SDL_FreeSurface(texte);
  }
}

void Label::onFocus(bool foc, bool nextIfUnactive)
{
  focused = foc;
  if(foc)
  {
    if(nextIfUnactive)
      container->focusNext();
    else
      container->focusPrev();
  }
}
