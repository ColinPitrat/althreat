#include "liste.h"

Liste::Liste(SDL_Rect position)
{
  // Style par défaut
  border = (SDL_Color) {150, 150, 150, 255};
  bg = (SDL_Color) {0, 0, 0, 255};
  fg = (SDL_Color) {255, 0, 0, 255};
  select = (SDL_Color) {180, 180, 180, 255};
  focusborder = (SDL_Color) {150, 150, 150, 255};
  focusbg = (SDL_Color) {0, 0, 0, 255};
  focusfg = (SDL_Color) {255, 255, 0, 255};
  focusselect = (SDL_Color) {80, 80, 80, 255};
  unactiveborder = (SDL_Color) {150, 150, 150, 255};
  unactivebg = (SDL_Color) {0, 0, 0, 255};
  unactivefg = (SDL_Color) {175, 175, 175, 255};
  unactiveselect = (SDL_Color) {80, 80, 80, 255};
  unactive = false;
  borderSize = 1;
  slideSize = 20;
  numFirst = 0;
  nbVals = 0;
  val = NULL;
  font = NULL;

  pos.x = position.x; pos.y = position.y; pos.w = position.w; pos.h = position.h;
}

Liste::~Liste()
{
  if(font)
  {
    TTF_CloseFont(font);
    font = NULL;
  }
  // Détruit la liste
  {
    std::list<std::string*>::iterator it = choix.begin();
    while(it != choix.end())
    {
      delete *it;
      it = choix.erase(it);
    }
  }
}

int Liste::filtre(const SDL_Event *event)
{
  if(!unactive)
  {
    // TODO : traiter les déplacements par pgup et pgdown
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
      if(event->button.x >= pos.x && event->button.x <= pos.w + pos.x && event->button.y >= pos.y && event->button.y <= pos.h + pos.y)
      {
        setFocus(true);
        // Traitement de la sélection à la souris
        // TODO : gérer la position de l'ascenseur
        if(event->button.x < pos.w + pos.x - slideSize)
        {
          std::list<std::string*>::iterator it;
		  it = first;
          int posY = pos.y;
          while(it != choix.end())
          {
            SDL_Surface *texte;
            texte = TTF_RenderText_Shaded(font, (*it)->c_str(), fg, bg);
            SDL_Rect itemPos;
            itemPos.x = pos.x; itemPos.y = posY; itemPos.h = texte->h; itemPos.w = texte->w;
            posY = itemPos.y + itemPos.h;
            if(event->button.y <= posY)
              break;
            it++;
            SDL_FreeSurface(texte);
          }
          val = *it;
        }
        if(event->button.x >= pos.w + pos.x - slideSize && event->button.y <= pos.y + slideSize)
        {
          prevValue();
        }
        if(event->button.x >= pos.w + pos.x - slideSize && event->button.y >= pos.y + pos.h - slideSize)
        {
          nextValue();
        }
      }
      else
        setFocus(false);
    }

    if(focused && event->type == SDL_KEYDOWN)
    {
      int key = event->key.keysym.sym;
      if (key == SDLK_UP)
      {
        prevValue();
      }
      if (key == SDLK_DOWN)
      {
        nextValue();
      }
    }
  }

  return 1;
}

int Liste::getIntValue()
{
  std::list<std::string*>::iterator it = find(choix.begin(), choix.end(), val);
  std::list<std::string*>::iterator it2 = choix.begin();
  int i = 0;
  while(it != choix.end() && (*it)!=(*it2))
  {
    i++;
    it2++;
  }
  if(it2 == choix.end())
    i = -1;
  return i;
}

void Liste::nextValue()
{
  std::list<std::string*>::iterator it = find(choix.begin(), choix.end(), val);
  if(it != choix.end())
  {
    it++;
    if(it != choix.end())
    {
      val = *it;
    }
  }
  else
  {
    it = choix.begin();
    val = *it;
  }
}

void Liste::prevValue()
{
  std::list<std::string*>::iterator it = find(choix.begin(), choix.end(), val);
  if(it != choix.begin() && it != choix.end())
  {
    if(it == first)
    {
      first--;
      numFirst--;
    }
    it--;
    val = *it;
  }
  else
  {
    it = choix.begin();
    val = *it;
  }
}

void Liste::afficher(SDL_Surface *Screen)
{
  SDL_Color *pborder, *pbg, *pfg, *pselect;
  if(unactive)
  {
    pborder = &unactiveborder;
    pbg = &unactivebg;
    pfg = &unactivefg;
    pselect = &unactiveselect;
  }
  else
  {
    if(focused)
    {
      pborder = &focusborder;
      pbg = &focusbg;
      pfg = &focusfg;
      pselect = &focusselect;
    }
    else
    {
      pborder = &border;
      pbg = &bg;
      pfg = &fg;
      pselect = &select;
    }
  }
  // TODO : ascenseur
  if(font)
  {
    if(borderSize > 0)
    {
      SDL_Rect bord;
      bord.x = pos.x - borderSize; bord.y = pos.y - borderSize; bord.w = pos.w + 2*borderSize; bord.h = pos.h + 2*borderSize;
      SDL_FillRect(Screen, &bord, SDL_MapRGB(Screen->format, pborder->r, pborder->g, pborder->b));
    }
    SDL_Rect tmpPos;
    tmpPos.x = pos.x; tmpPos.y = pos.y; tmpPos.w = pos.w - slideSize - borderSize; tmpPos.h = pos.h;
    SDL_FillRect(Screen, &tmpPos, SDL_MapRGB(Screen->format, pbg->r, pbg->g, pbg->b));
    tmpPos.x = pos.x + pos.w - slideSize; tmpPos.w = slideSize;
    SDL_FillRect(Screen, &tmpPos, SDL_MapRGB(Screen->format, pbg->r, pbg->g, pbg->b));
    tmpPos.h = slideSize;
    SDL_FillRect(Screen, &tmpPos, SDL_MapRGB(Screen->format, pfg->r, pfg->g, pfg->b));
    tmpPos.y = pos.y + pos.h - slideSize;
    SDL_FillRect(Screen, &tmpPos, SDL_MapRGB(Screen->format, pfg->r, pfg->g, pfg->b));

    // TODO : ajouter un ascenseur
    std::list<std::string*>::iterator it;
	it = first;
    int posY = pos.y;
    int i = numFirst;
    int n = numFirst;
    while(it != choix.end())
    {
      SDL_Surface *texte;
      SDL_Rect itemPos;
      SDL_Rect itemSrc;
      // TODO : alignement
      if(*it == val)
        texte = TTF_RenderText_Shaded(font, (*it)->c_str(), *pfg, *pselect);
      else
        texte = TTF_RenderText_Shaded(font, (*it)->c_str(), *pfg, *pbg);
      itemSrc.x = 0; itemSrc.y = 0; itemSrc.h = (texte->h < pos.h + pos.y - posY) ? texte->h : pos.h + pos.y - posY; itemSrc.w = (texte->w < pos.w - slideSize - borderSize) ? texte->w : pos.w - slideSize - borderSize;
      itemPos.x = pos.x; itemPos.y = posY; itemPos.h = itemSrc.h; itemPos.w = pos.w - slideSize - borderSize;
      if(*it == val)
      {
        SDL_FillRect(Screen, &itemPos, SDL_MapRGB(Screen->format, pselect->r, pselect->g, pselect->b));
        n = i;
      }
      i++;
      itemPos.w = itemSrc.w;
      SDL_BlitSurface(texte, &itemSrc, Screen, &itemPos);
      SDL_FreeSurface(texte);
      posY = itemPos.y + itemPos.h;
      if(posY >= pos.y + pos.h)
      {
        if(*it == val)
        {
		  first++;
          numFirst++;
        }
        break;
      }
      it++;
    }
    tmpPos.h = (pos.h - 2*slideSize) / nbVals;
    tmpPos.y = pos.y + slideSize + tmpPos.h * n;
    SDL_FillRect(Screen, &tmpPos, SDL_MapRGB(Screen->format, pselect->r, pselect->g, pselect->b));
  }
}

void Liste::onFocus(bool foc, bool nextIfUnactive)
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
