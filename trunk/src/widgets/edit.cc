#include "edit.h"

Edit::Edit(SDL_Rect position,  std::string *value, Alignement align)
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
  val = value;
  alignement = align;
}

Edit::~Edit()
{
  if(font)
  {
    TTF_CloseFont(font);
    font = NULL;
  }
}

int Edit::filtre(const SDL_Event *event)
{
  if(!unactive)
  {
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
      if(event->button.x >= pos.x && event->button.x <=(pos.w + pos.x) && event->button.y >= pos.y && event->button.y <= (pos.h + pos.y))
        setFocus(true);
      else
        setFocus(false);
    }


    if(focused && event->type == SDL_KEYDOWN)
    {
      int key = event->key.keysym.sym;
      int ch = event->key.keysym.unicode;
      if (key == SDLK_BACKSPACE)
      {
        if(val->length())
          *val = val->substr(0, val->length() - 1);
        return 0;
      }
      if (ch >= 32 && ch <= 126)
      {
        (*val) += (char) ch;
        return 0;
      }
    }
  }

  return 1;
}

void Edit::afficher(SDL_Surface *Screen)
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
    
    SDL_Surface *texte = NULL;
    SDL_Rect destRect;
    SDL_Rect srcRect;
    if(focused)
    {
      (*val) += "_";
      texte = TTF_RenderText_Shaded(font, val->c_str(), *pfg, *pbg);
      *val = val->substr(0,val->length() - 1);
      switch(alignement)
      {
        // TODO : Traiter les cas autres que LEFT
        case T3F_RIGHT:
          break;
        case T3F_CENTER:
          break;
        case T3F_LEFT:
        default:
          destRect.x = pos.x; destRect.w = texte->w > pos.w ? pos.w : texte->w; 
          destRect.y = (texte->h < pos.h) ? pos.y + (pos.h - texte->h) / 2: pos.y; destRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          srcRect.w = texte->w > pos.w ? pos.w : texte->w; srcRect.x = texte->w - srcRect.w; 
          srcRect.y = (texte->h < pos.h) ? 0 : (texte->h -pos.h) / 2; srcRect.h = (texte->h < pos.h) ? texte->h : pos.h;
          break;
      }
      SDL_BlitSurface(texte, &srcRect, Screen, &destRect);
    }
    else
    {
      if(val->length())
      {
        texte = TTF_RenderText_Shaded(font, val->c_str(), fg, bg);
        switch(alignement)
        {
          // TODO : Traiter les cas autres que LEFT
          case T3F_RIGHT:
            break;
          case T3F_CENTER:
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
    }
    SDL_FreeSurface(texte);
  }
}

void Edit::onFocus(bool foc, bool nextIfUnactive)
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
