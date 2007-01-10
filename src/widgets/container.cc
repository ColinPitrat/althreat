#include "container.h"

Container::Container()
{
}

Container::~Container()
{
  // Il y a un choix à faire : la destruction du container engendre la destruction de son contenu ou non
  // Pour l'instant, on choisit de ne pas détruire le contenu. Pour une bonne utilisation, il faut faire appel
  // à clear ou a deleteAll avant de détruire le conteneur.
  liste.clear();
}

int Container::filtre(const SDL_Event *event)
{
  typeliste::iterator i = liste.begin();
  while(i != liste.end())
  {
    if(!((*i)->filtre(event)))
      return 0;
    ++i;
  }
  return 1;
}

void Container::afficher(SDL_Surface *Screen)
{
  typeliste::iterator i = liste.begin();
  while(i != liste.end())
  {
    (*i)->afficher(Screen);
    ++i;
  }
}

void Container::deleteAll()
{
  // TODO : verifier si le widget est un conteneur, et faire un deleteAll
  // dessus le cas écheant
  typeliste::iterator i = liste.begin();
  while(i != liste.end())
  {
    delete *i;
    i = liste.erase(i);
  }
}

bool Container::containFocusableWidget()
{
  for(typeliste::iterator i = liste.begin(); i != liste.end(); i++)
  {
    if((*i)->isFocusable())
      return true;
  }
  return false;
}

void Container::onFocus(bool foc, bool)
{
  focused = foc;
}
