#include	"Bot.hh"

Bot::Bot()
  : _action(NULL), _battle(NULL), _battleState(false), _state(0)
{
}

Bot::~Bot()
{
}

void		Bot::_saveKeyState()
{
  _keyState = 0;
  for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
    if (sdlGetButton((EKey) i))
      _keyState |= (1 << i);
}

void		Bot::_loadKeyState()
{
  for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
    sdlSetButton((EKey) i, (_keyState >> i) & 1);
  _keyState = 0;
}

void		Bot::update()
{
  if (Action::data->inBattle() != _battleState)
    (this->*(!_battleState ? &Bot::_saveKeyState : &Bot::_loadKeyState))();
  _battleState = Action::data->inBattle();
  if (_battleState)
    {
      if (_battle)
	_battle->update();
      return;
    }
  if (!_action)
    {
      if (_queue.size())
	{
	  _action = _queue.front();
	  _queue.pop_front();
	}
      else
	_state = Action::NOT_STARTED;
    }
  if (_action)
    {
      _action->update();
      _state = _action->getState();
      if (_state > Action::RUNNING)
	{
	  delete _action;
	  _action = NULL;
	}
    }
  _battleState = Action::data->inBattle();
}

void		Bot::queue(AAction *action)
{
  _queue.push_back(action);
}
