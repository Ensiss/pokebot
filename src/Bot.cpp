#include	"Bot.hh"

Bot::Bot()
  : _action(NULL), _state(0)
{
}

Bot::~Bot()
{
}

void		Bot::update()
{
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
}

void		Bot::queue(AAction *action)
{
  _queue.push_back(action);
}
