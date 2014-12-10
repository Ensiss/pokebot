#include	"Action/ChangeMap.hh"

/**
   Go to a different map by going to the edge

   @param       t       Type of the connection to use
 */
Action::ChangeMap::ChangeMap(ConnectType t)
  : _type(t)
{
}

Action::ChangeMap::~ChangeMap()
{
}

void		Action::ChangeMap::_init()
{
  const World::Map      &m = _data.getCurrentMap();
  bool          found = false;
  int           dx, dy, sx, sy, ex, ey;
  EKey          keys[4] = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

  for (uint8_t i = 0; !found && i < m.getNbConnections(); i++)
    {
      if (m.getConnection(i).getType() == _type)
        found = true;
    }
  if (!found)
    {
      fprintf(stderr, "No connection of type %d on this map\n", _type);
      _state = Action::ERROR;
      return;
    }
  if (_type >= CO_DIVE)
    {
      fprintf(stderr, "Connection type %d is not implemented yet\n", _type);
      _state = Action::ERROR;
      return;
    }
  sx = (_type == CO_RIGHT) * (m.getWidth() - 1);
  sy = (_type == CO_DOWN) * (m.getHeight() - 1);
  dx = (_type <= CO_UP);
  dy = (_type > CO_UP);
  ex = dx * m.getWidth();
  ey = dy * m.getHeight();
  while (sx != ex || sy != ey)
    {
      if (m.getNode(sx, sy).getStatus() == 0x0C)
        {
          queue(new Action::MoveTo(sx, sy));
          queue(new Action::TurnDirection(keys[_type - 1]));
          queue(new Action::PressButton(keys[_type - 1]));
          break;
        }
      sx += dx;
      sy += dy;
    }
}

void		Action::ChangeMap::_update()
{
  _state = Action::FINISHED;
}
