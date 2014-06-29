#include	"Movement.hh"

void		Movement::_releaseKeys()
{
  for (uint8_t i = KEY_LEFT; i <= KEY_DOWN; i++)
    sdlSetButton((EKey) i, false);
}

void		Movement::update()
{
  int		x = _data.player().getX();
  int		y = _data.player().getY();
  bool		moved = _oldx != x || _oldy != y;

  if (_path && _pathi == _path->size() && moved)
    {
      _releaseKeys();
      delete _path;
      _path = NULL;
    }
  else if (_path && _pathi < _path->size() && (_pathi == 1 || moved))
    {
      int	dx = (*_path)[_pathi]->x - x;
      int	dy = (*_path)[_pathi]->y - y;
      EKey	k;

      _releaseKeys();
      if (!dx)
	k = dy < 0 ? KEY_UP : KEY_DOWN;
      else
	k = dx < 0 ? KEY_LEFT : KEY_RIGHT;
      sdlSetButton(k, true);
      _pathi++;
    }

  _oldx = x;
  _oldy = y;
}

void		Movement::moveTo(uint16_t x, uint16_t y)
{
  Player	&p = _data.player();
  World::Map	&m = _data.world()[p.getBank()][p.getMap()];

  if (_path)
    delete _path;
  _tx = x;
  _ty = y;
  _oldx = p.getX();
  _oldy = p.getY();
  _path = m.findPath(_oldx, _oldy, _tx, _ty);
  _pathi = 1;
}
