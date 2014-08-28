#include	"ActionMoveTo.hh"

Action::MoveTo::MoveTo(uint16_t x, uint16_t y)
  : _data(*Action::data), _tx(x), _ty(y)
{
  _path = NULL;
  _pathi = 1;
  _state = NOT_STARTED;
}

Action::MoveTo::~MoveTo()
{
}

void		Action::MoveTo::_releaseKeys()
{
  for (uint8_t i = KEY_LEFT; i <= KEY_DOWN; i++)
    sdlSetButton((EKey) i, false);
}

void		Action::MoveTo::_init()
{
  Player	&p = _data.player();
  World::Map	&m = _data.world()[p.getBank()][p.getMap()];

  _oldx = p.getX();
  _oldy = p.getY();
  if (p.getX() == _tx && p.getY() == _ty)
    {
      _state = Action::FINISHED;
      return;
    }
  _path = m.findPath(p.getX(), p.getY(), _tx, _ty);
  _state = _path ? Action::RUNNING : Action::ERROR;
}

void		Action::MoveTo::_update()
{
  Player	&p = _data.player();
  World::Map	&m = _data.world()[p.getBank()][p.getMap()];
  bool		moved = _oldx != p.getX() || _oldy != p.getY();

  if (_path && _pathi == _path->size() && moved)
    {
      _state = Action::FINISHED;
      _releaseKeys();
      delete _path;
      _path = NULL;
    }
  else if (_path && _pathi < _path->size() && (_pathi == 1 || moved))
    {
      int	dx = (*_path)[_pathi]->x - p.getX();
      int	dy = (*_path)[_pathi]->y - p.getY();
      EKey	k;

      _releaseKeys();
      if (!dx)
	k = dy < 0 ? KEY_UP : KEY_DOWN;
      else
	k = dx < 0 ? KEY_LEFT : KEY_RIGHT;
      sdlSetButton(k, true);
      _pathi++;
    }

  _oldx = p.getX();
  _oldy = p.getY();
}
