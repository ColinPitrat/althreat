#include "view.h"

View *View::view = NULL;

View::View()
{
  if(!TTF_WasInit() && TTF_Init()==-1) 
    std::cout << "Error : SDL_ttf initialization : " << TTF_GetError() << std::endl;
  SDL_EnableUNICODE(1);
  SDL_SetEventFilter(View::_filtre);
  view = this;
}

View::~View()
{
  TTF_Quit();
  view = NULL;
}

int View::_filtre(const SDL_Event *event)
{
  if(view)
  {
    typeliste::iterator i = view->liste.begin();
    while(i != view->liste.end())
    {
      if(!((*i)->filtre(event)))
        return 0;
      ++i;
    }
  }
  return 1;
}

void View::_afficher(SDL_Surface *Screen)
{
  if(view)
  {
    typeliste::iterator i = view->liste.begin();
    while(i != view->liste.end())
    {
      (*i)->afficher(Screen);
      ++i;
    }
  }
}

void View::onFocus(bool foc, bool)
{
  focused = foc;
}
