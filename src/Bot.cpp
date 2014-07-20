#include	"Bot.hh"

Bot::Bot()
  : _action(NULL)
{
}

Bot::~Bot()
{
}

void		Bot::update()
{
  if (!_action && _queue.size())
    {
      _action = _queue.front();
      _queue.pop_front();
    }
  if (_action)
    {
      _action->update();
    }
}

void		Bot::queue(AAction *action)
{
  _queue.push_back(action);
}
