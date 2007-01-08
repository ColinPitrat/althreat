#include "widget.h"
#include "focuscontainer.h"

void Widget::setFocus(bool foc)
{
  if(foc && container)
  { 
    if(!focused) 
    {
      container->focus(this);
      focused = true;
    }
  }
  else
  {
    focused = false;
  }
}
