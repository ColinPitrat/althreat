#ifndef _ALTHREAT_JOYSTICK
#define _ALTHREAT_JOYSTICK

struct joystick_event
{
  Uint8 type;   // SDL_AXISMOTION, SDL_JOYBUTTONUP or SDL_JOYBUTTONDOWN
  Uint8 which;  // joystick ID
  Uint8 button_or_axis;
  Sint16 value;  // axis value or button state
};

#endif
// vim:shiftwidth=2:shiftround:expandtab:cindent
