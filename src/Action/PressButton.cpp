#include	"Action/PressButton.hh"

Action::PressButton::PressButton(EKey key)
  : _key(key)
{
}

Action::PressButton::~PressButton()
{
}

void		Action::PressButton::_init()
{
  _i = 0;
}

void		Action::PressButton::_update()
{
  if (!_i)
    {
      for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
        if (i != KEY_BUTTON_SPEED)
          sdlSetButton((EKey) i, false);
    }
  else if (_i == 1)
    sdlSetButton(_key, true);
  else
    _state = Action::FINISHED;
  _i++;
}
