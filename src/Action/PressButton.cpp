#include	"ActionPressButton.hh"

Action::PressButton::PressButton(EKey key)
  : _key(key)
{
}

Action::PressButton::~PressButton()
{
}

void		Action::PressButton::_init()
{
  _first = true;
}

void		Action::PressButton::_update()
{
  for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
    if (i != KEY_BUTTON_SPEED)
      sdlSetButton((EKey) i, false);
  if (_first)
    {
      _first = false;
      return;
    }
  sdlSetButton(_key, true);
  _state = Action::FINISHED;
}
