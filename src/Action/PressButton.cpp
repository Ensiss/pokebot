#include	"Action/PressButton.hh"

/**
   Press a key

   This action releases all keys, and then presses the specified key

   @param key   The key to press
 */
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
  if (!getCounter())
    {
      for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
        if (i != KEY_BUTTON_SPEED)
          sdlSetButton((EKey) i, false);
    }
  else if (getCounter() == 1)
    sdlSetButton(_key, true);
  else
    _state = Action::FINISHED;
}
