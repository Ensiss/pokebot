#include	"Action/UseWarp.hh"

Action::UseWarp::UseWarp(uint8_t warpId)
  : _id(warpId), _i(0)
{
}

Action::UseWarp::~UseWarp()
{
}

void		Action::UseWarp::_init()
{
  Player        &p = _data.player();
  World::Map    &m = _data.world()[p.getBank()][p.getMap()];
  int           x = m.warps[_id].x;
  int           y = m.warps[_id].y;
  int           status = m.data[y][x].status;

  queue(new Action::MoveTo(x, y, status != 0x0C));
}

void		Action::UseWarp::_update()
{
  Player        &p = _data.player();
  World::Map    &m = _data.world()[p.getBank()][p.getMap()];
  int           tx = m.warps[_id].x;
  int           ty = m.warps[_id].y;
  int           px = p.getX();
  int           py = p.getY();
  EKey          key;

  if (!_i)
    {
      key = (py < ty ? KEY_DOWN : py > ty ? KEY_UP : px > tx ? KEY_LEFT : KEY_RIGHT);
      queue(new Action::PressButton(key));
      queue(new Action::PressButton(key));
    }
  else
    _state = Action::FINISHED;
  _i++;
}
