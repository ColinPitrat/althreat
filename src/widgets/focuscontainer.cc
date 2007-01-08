#include "focuscontainer.h"

FocusContainer::FocusContainer()
{
  focusedWidget = NULL;
}

FocusContainer::~FocusContainer()
{
}

int FocusContainer::filtre(const SDL_Event *event)
{
  if(event->type == SDL_KEYDOWN)
  {
    if(event->key.keysym.sym == SDLK_TAB)
    {
      if(event->key.keysym.mod & KMOD_SHIFT)
      {
        focusPrev();
        return 0;
      }
      if(event->key.keysym.mod == KMOD_NONE)
      {
        focusNext();
        return 0;
      }
    }
  }
  Container::filtre(event);
  return 1;
}

void FocusContainer::focusNext()
{
  typeliste::iterator i;
  if(focusedWidget)
  {
    focusedWidget->onFocus(false);
    i = find(liste.begin(), liste.end(), focusedWidget);
    if(i == liste.end())
      i = liste.begin();
    i++;
    if(i == liste.end())
      i = liste.begin();
  }
  else
  {
    i = liste.begin();
  }
  focusedWidget = *i;
  focusedWidget->onFocus(true);
}

void FocusContainer::focusPrev()
{
  typeliste::iterator i;
  if(focusedWidget)
  {
    focusedWidget->onFocus(false);
    i = find(liste.begin(), liste.end(), focusedWidget);
    if(i == liste.begin())
      i = liste.end();
    i--;
  }
  else
  {
    i = liste.end();
    i--;
  }
  focusedWidget = *i;
  focusedWidget->onFocus(true, false);
}

bool FocusContainer::focus(Widget *w)
{
  typeliste::iterator i;
  i = find(liste.begin(), liste.end(), w);
  if(i != liste.end())
  {
    if(focusedWidget)
      focusedWidget->onFocus(false);
    focusedWidget = *i;
    focusedWidget->onFocus(true);
    return true;
  }
  return false;
}

void FocusContainer::onFocus(bool foc, bool)
{
  focused = foc;
}
