#include	"Action/UseWarp.hh"

std::map<uint16_t, EKey>     Action::UseWarp::_behaviours = {
  {0x62, KEY_RIGHT},    // Warp to block right
  {0x63, KEY_LEFT},     // Warp to block left
  {0x64, KEY_UP},       // Warp to block up
  {0x65, KEY_DOWN},     // Warp to block down
  {0x6A, KEY_LEFT},     // Pokecenter stairs up
  {0x6B, KEY_RIGHT},    // Pokecenter stairs down
  {0x6C, KEY_RIGHT},    // Stairs up right
  {0x6D, KEY_RIGHT},    // Stairs down right
  {0x6E, KEY_LEFT},     // Stairs up left
  {0x6F, KEY_LEFT},     // Stairs down left
};

/**
   Warp to a different map

   @param warpId        Index of the warp in the warps array
 */
Action::UseWarp::UseWarp(uint8_t warpId)
  : _id(warpId)
{
}

Action::UseWarp::~UseWarp()
{
}

void		Action::UseWarp::_init()
{
  Player        &p = _data.getPlayer();
  World::Map    &m = _data.getWorld()[p.getBankId()][p.getMapId()];

  if (_id >= m.getNbWarps())
    {
      fprintf(stderr, "%d is not a valid warp ID\n", _id);
      _state = Action::ERROR;
      return;
    }

  const World::WarpEvt &warp = m.getWarp(_id);
  int           x = warp.getX();
  int           y = warp.getY();
  int           status = m.getNode(x, y).getStatus();

  queue(new Action::MoveTo(x, y, status == 0x01));
}

void		Action::UseWarp::_update()
{
  Player        &p = _data.getPlayer();
  World::Map    &m = _data.getWorld()[p.getBankId()][p.getMapId()];
  const World::WarpEvt &warp = m.getWarp(_id);
  int           tx = warp.getX();
  int           ty = warp.getY();
  int           px = p.getX();
  int           py = p.getY();
  EKey          key = (EKey) -1;

  if (!getCounter())
    {
      if (px == tx && py == ty)
        {
          std::map<uint16_t, EKey>::iterator    it = _behaviours.find(m.getNode(tx, ty).getBehavior());
          if (it != _behaviours.end())
            key = (*it).second;
        }
      else
        key = (py < ty ? KEY_DOWN : py > ty ? KEY_UP : px > tx ? KEY_LEFT : KEY_RIGHT);
      if (key != (EKey) -1)
        {
          queue(new Action::PressButton(key));
          queue(new Action::PressButton(key));
          queue(new Action::Wait(100));
        }
    }
  else
    _state = Action::FINISHED;
}
