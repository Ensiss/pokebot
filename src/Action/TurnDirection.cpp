#include	"Action/TurnDirection.hh"

/**
   Turn in a direction

   @param key   The key corresponding to the direction
 */
Action::TurnDirection::TurnDirection(EKey key)
  : _key(key)
{
}

Action::TurnDirection::~TurnDirection()
{
}

void		Action::TurnDirection::_init()
{
  int           dir = _data.overWorld(0).getDir();
  EKey          dirKey[4] = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

  if (dirKey[dir - 1] != _key)
    {
      queue(new Action::PressButton(_key));
      queue(new Action::Wait(5));
    }
}

void		Action::TurnDirection::_update()
{
  _state = Action::FINISHED;
}
