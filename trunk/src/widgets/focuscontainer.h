#ifndef _T3F_SDL_FOCUSCONTAINER
#define _T3F_SDL_FOCUSCONTAINER

#include <algorithm>
#include "container.h"

typedef std::list<Widget*> typeliste;

class FocusContainer: public Container
{
  public:
    FocusContainer();
    ~FocusContainer();
    void focusNext();
    void focusPrev();
    bool focus(Widget *w);
    virtual void addWidget(Widget *widget) { liste.push_back(widget); widget->setContainer(this); };
    virtual int filtre(const SDL_Event *event);
    virtual void onFocus(bool foc, bool nextIfUnactive = true);

  private:
    Widget *focusedWidget;
};

#endif
