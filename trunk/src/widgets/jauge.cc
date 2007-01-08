#include "jauge.h"

Jauge::Jauge(SDL_Rect position, int setmin, int setmax, int *value, Orientation orient)
{
  // Style par défaut
  SDL_Surface *Screen = SDL_GetVideoSurface();
  border = SDL_MapRGB(Screen->format, 150, 150, 150);
  bg = SDL_MapRGB(Screen->format, 0, 0, 0);
  fg = SDL_MapRGB(Screen->format, 255, 0, 0);
  focusborder = SDL_MapRGB(Screen->format, 150, 150, 150);
  focusbg = SDL_MapRGB(Screen->format, 0, 0, 0);
  focusfg = SDL_MapRGB(Screen->format, 255, 255, 0);
  unactiveborder = SDL_MapRGB(Screen->format, 150, 150, 150);
  unactivebg = SDL_MapRGB(Screen->format, 0, 0, 0);
  unactivefg = SDL_MapRGB(Screen->format, 175, 175, 175);
  unactive = false;
  borderSize = 1;

  pos.x = position.x; pos.y = position.y; pos.w = position.w; pos.h = position.h;
  min = setmin; max = setmax; val = value;
  inuse = false;
  orientation = orient;
}

void Jauge::setColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder)
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  bg = SDL_MapRGB(Screen->format, setbg.r, setbg.g, setbg.b); 
  fg = SDL_MapRGB(Screen->format, setfg.r, setfg.g, setfg.b);
  border = SDL_MapRGB(Screen->format, setborder.r, setborder.g, setborder.b); 
}

void Jauge::setFocusColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder)
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  focusbg = SDL_MapRGB(Screen->format, setbg.r, setbg.g, setbg.b); 
  focusfg = SDL_MapRGB(Screen->format, setfg.r, setfg.g, setfg.b);
  focusborder = SDL_MapRGB(Screen->format, setborder.r, setborder.g, setborder.b); 
}

void Jauge::setUnactiveColors(SDL_Color setbg, SDL_Color setfg, SDL_Color setborder)
{
  SDL_Surface *Screen = SDL_GetVideoSurface();
  unactivebg = SDL_MapRGB(Screen->format, setbg.r, setbg.g, setbg.b); 
  unactivefg = SDL_MapRGB(Screen->format, setfg.r, setfg.g, setfg.b);
  unactiveborder = SDL_MapRGB(Screen->format, setborder.r, setborder.g, setborder.b); 
}

int Jauge::filtre(const SDL_Event *event)
{
  if(!unactive)
  {
    switch(orientation)
    {
      case T3F_VERT:
        // Si on clique sur la jauge, on fixe la valeur et on donne l'activitée à cette jauge
        // On peut quand même continuer à traiter l'évenement
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) 
          if(event->button.x >= pos.x && event->button.x <= (pos.w + pos.x) && event->button.y >= pos.y && event->button.y <= (pos.h + pos.y))
          {
            *val = min + (pos.y + pos.h - event->button.y) * (max - min) / pos.h;
            inuse = true;
            setFocus(true);
          }

        // Si on relache le bouton gauche et que la jauge avait l'activitée, elle le perd
        // On peut quand même continuer à traiter l'évenement
        if (inuse && event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) 
          inuse = false;

        // Si la jauge à l'activitée et que l'on déplace la souris, on modifie la valeure de la jauge
        // L'évenement est attrapé
        if (inuse && event->type == SDL_MOUSEMOTION && (event->motion.state & SDL_BUTTON(1)))
        {
          if(event->button.y <= pos.y)
            *val = max;
          else if(event->button.y >= (pos.h + pos.y))
            *val = min;
          else 
            *val = min + (pos.y + pos.h - event->motion.y) * (max - min) / pos.h;
          return 0;
        }

        if(focused && event->type == SDL_KEYDOWN)
        {
          if(event->key.keysym.sym == SDLK_PAGEUP)
          {
            if(*val < (max - (max - min) / 5))
              (*val) += (max - min) / 5;
            else
              (*val) = max;
          }
          if(event->key.keysym.sym == SDLK_PAGEDOWN)
          {
            if(*val > min + (max -min) / 5)
              (*val) -= (max - min) / 5;
            else
              (*val) = min;
          }
          if(event->key.keysym.sym == SDLK_UP)
          {
            if(*val < max)
              (*val)++;
          }
          if(event->key.keysym.sym == SDLK_DOWN)
          {
            if(*val > min)
              (*val)--;
          }
        }
        break;
      case T3F_HOR:
      default:
        // Si on clique sur la jauge, on fixe la valeur et on donne l'activitée à cette jauge
        // On peut quand même continuer à traiter l'évenement
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) 
          if(event->button.x >= pos.x && event->button.x <= (pos.w + pos.x) && event->button.y >= pos.y && event->button.y <= (pos.h + pos.y))
          {
            *val = min + (event->button.x - pos.x) * (max - min) / pos.w;
            setFocus(true);
            inuse = true;
          }

        // Si on relache le bouton gauche et que la jauge avait l'activitée, elle le perd
        // On peut quand même continuer à traiter l'évenement
        if (inuse && event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) 
          inuse = false;

        // Si la jauge à l'activitée et que l'on déplace la souris, on modifie la valeure de la jauge
        // L'évenement est attrapé
        if (inuse && event->type == SDL_MOUSEMOTION && (event->motion.state & SDL_BUTTON(1)))
        {
          if(event->button.x <= pos.x)
            *val = min;
          else if(event->button.x >= (pos.w + pos.x))
            *val = max;
          else 
            *val = min + (event->motion.x - pos.x) * (max - min) / pos.w;
          return 0;
        }

        if(focused && event->type == SDL_KEYDOWN)
        {
          if(event->key.keysym.sym == SDLK_PAGEUP)
          {
            if(*val < (max - (max - min) / 5))
              (*val) += (max - min) / 5;
            else
              (*val) = max;
          }
          if(event->key.keysym.sym == SDLK_PAGEDOWN)
          {
            if(*val > min + (max -min) / 5)
              (*val) -= (max - min) / 5;
            else
              (*val) = min;
          }
          if(event->key.keysym.sym == SDLK_RIGHT || event->key.keysym.sym == SDLK_UP)
          {
            if(*val < max)
              (*val)++;
          }
          if(event->key.keysym.sym == SDLK_LEFT || event->key.keysym.sym == SDLK_DOWN )
          {
            if(*val > min)
              (*val)--;
          }
        }
        break;
    }
  }
  return 1;
}

void Jauge::afficher(SDL_Surface *Screen)
{
  SDL_Rect foreground;
  if(borderSize > 0)
  {
    SDL_Rect bord;
    bord.x = pos.x - borderSize; bord.y = pos.y - borderSize; bord.w = pos.w + 2*borderSize; bord.h = pos.h + 2*borderSize;
    if(unactive)
      SDL_FillRect(Screen, &bord, unactiveborder);
    else
    {
      if(focused)
        SDL_FillRect(Screen, &bord, focusborder);
      else
        SDL_FillRect(Screen, &bord, border);
    }
  }
  switch(orientation)
  {
    case T3F_VERT:
      foreground.x = pos.x; foreground.w = pos.w; foreground.h = (*val - min) * pos.h / (max - min); foreground.y = pos.y + pos.h - foreground.h; 
      break;
    case T3F_HOR:
    default:
      foreground.x = pos.x; foreground.y = pos.y; foreground.w = (*val - min) * pos.w / (max - min); foreground.h = pos.h; 
      break;
  }
  if(unactive)
  {
    SDL_FillRect(Screen, &pos, unactivebg);
    SDL_FillRect(Screen, &foreground, unactivefg);
  }
  else
  {
    if(focused)
    {
      SDL_FillRect(Screen, &pos, focusbg);
      SDL_FillRect(Screen, &foreground, focusfg);
    }
    else
    {
      SDL_FillRect(Screen, &pos, bg);
      SDL_FillRect(Screen, &foreground, fg);
    }
  }
}

void Jauge::onFocus(bool foc, bool nextIfUnactive)
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
