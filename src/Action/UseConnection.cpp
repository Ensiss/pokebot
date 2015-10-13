#include	"Action/UseConnection.hh"

/**
   Change to another map using a connection

   @param dir   The direction of the next screen
 */
Action::UseConnection::UseConnection(uint8_t dir)
  : _dir(dir)
{
}

Action::UseConnection::~UseConnection()
{
}

void		Action::UseConnection::_init()
{
  EKey          dirToKey[] = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};
  Player        &p = _data.getPlayer();
  World::Map    &m = _data.getWorld()[p.getBankId()][p.getMapId()];

  if (_dir == CO_NONE || _dir > CO_RIGHT)
    {
      fprintf(stderr, "Only _directions Up, Down, Left and Right are currently suported\n");
      _state = Action::ERROR;
      return;
    }
  bool          vtcl = _dir == CO_UP || _dir == CO_DOWN;
  uint32_t      x = (_dir == CO_RIGHT ? m.getWidth() - 1 : 0);
  uint32_t      y = (_dir == CO_DOWN ? m.getHeight() - 1 : 0);
  int           xstep = vtcl;
  int           ystep = !vtcl;

  while (x < m.getWidth() && y < m.getHeight())
    {
      if (m.getNode(x, y).getStatus() == 0x0C) // Empty node, we should be able to cross there
        break;
      x += xstep;
      y += ystep;
    }
  if (x >= m.getWidth() || y >= m.getHeight())
    {
      fprintf(stderr, "Couldn't find an exit in the specified direction\n");
      _state = Action::ERROR;
      return;
    }
  _tx = x;
  _ty = y;
  _keyDir = dirToKey[_dir - 1];
  _origBank = p.getBankId();
  _origMap = p.getMapId();
  queue(new Action::MoveTo(_tx, _ty));
}

void		Action::UseConnection::_update()
{
  Player        &p = _data.getPlayer();

  if (p.getBankId() != _origBank || p.getMapId() != _origMap)
    _state = Action::FINISHED;
  else if (p.getX() != _tx || p.getY() != _ty)
    emit("onInit");
  else
    sdlSetButton(_keyDir, getCounter() % 2);
}
