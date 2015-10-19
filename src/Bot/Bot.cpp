#include	"Bot.hh"

Bot             Bot::bot;

Bot::Bot()
{
}

Bot::~Bot()
{
  clear();
}

void		Bot::update()
{
  if (_queue.size())
    {
      if (_queue.front()() == -1)
        _queue.pop_front();
    }
}

void		Bot::queue(LuaRef action)
{
  if (!action.isNil() && action.isFunction())
    _queue.push_back(action);
}

void		Bot::clear()
{
  _queue.clear();
  for (uint8_t i = KEY_LEFT; i < KEY_BUTTON_SPEED; i++)
    sdlSetButton((EKey) i, false);
}
