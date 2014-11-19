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
}

void		Action::PressButton::_update()
{
  if (!_getCounter())
    {
      for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
        if (i != KEY_BUTTON_SPEED)
          sdlSetButton((EKey) i, false);
    }
  else if (_getCounter() == 1)
    sdlSetButton(_key, true);
  else
    _state = Action::FINISHED;
}
