#include	"Action/MoveTo.hh"

/**
   Move to a location

   @param x,y           Coordinates of the destination
   @param allowApprox   If the bot should stop 1 tile before the destination
 */
Action::MoveTo::MoveTo(uint16_t x, uint16_t y, bool allowApprox)
  : _owInit(false), _approx(allowApprox), _tx(x), _ty(y), _tid(-1), _path(NULL)
{
  addListener("onFrame", &Action::MoveTo::_checkNPCMovement);
}

/**
   Move to a person

   @param tid           Index of the person in the person array
 */
Action::MoveTo::MoveTo(uint8_t tid)
  : _owInit(false), _approx(true), _tx(0), _ty(0), _tid(tid), _path(NULL)
{
  addListener("onFrame", &Action::MoveTo::_checkNPCMovement);
}

Action::MoveTo::~MoveTo()
{
}

void		Action::MoveTo::addListener(const std::string &signal, void (Action::MoveTo::*listener)())
{
  std::function<void (AAction *)> l = [listener](AAction *that){(static_cast<Action::MoveTo *>(that)->*listener)();};

  _listeners[signal].push_back(l);
}

void		Action::MoveTo::_checkNPCMovement()
{
  const OverWorld	*ows = _data.overWorlds();

  for (int i = 1; _owInit && i < 16 && (ows[i].getMap() || ows[i].getBank()); i++)
    {
      if (ows[i].getBank() == _data.player().getBank() &&	// If the overworld is in our map
	  ows[i].getMap() == _data.player().getMap() &&
	  (ows[i].getDestX() != _oldow[i].getDestX() ||		// If the overworld is moving
	   ows[i].getDestY() != _oldow[i].getDestY() ||
	   ows[i].getBank() != _oldow[i].getBank() ||		// Or if it's a new overworld
	   ows[i].getMap() != _oldow[i].getMap()))
	emit("onInit");
    }
  memcpy((void *) (_oldow + 1), (void *) (ows + 1), 15 * sizeof(OverWorld));
  _owInit = true;
}

void		Action::MoveTo::_releaseKeys()
{
  for (uint8_t i = KEY_LEFT; i <= KEY_DOWN; i++)
    sdlSetButton((EKey) i, false);
}

void            Action::MoveTo::_updateTargetPos()
{
  if (_tid == -1)
    return;

  const OverWorld	*ows = _data.overWorlds();
  Player	&p = _data.player();
  World::Map    &m = _data.world()[p.getBank()][p.getMap()];

  _tx = m.persons[_tid].x;
  _ty = m.persons[_tid].y;
  for (int i = 1; i < 16 && (ows[i].getMap() || ows[i].getBank()); i++)
    {
      if (ows[i].getEventNb() == m.persons[_tid].evtNb)
        {
          _tx = ows[i].getDestX();
          _ty = ows[i].getDestY();
          return;
        }
    }
}

void            Action::MoveTo::_searchBehindBar()
{
  Player	&p = _data.player();
  World::Map    &m = _data.world()[p.getBank()][p.getMap()];

  for (int i = 0; i < 4; i++)
    {
      int       x = _tx + !(i & 1) * (i - 1);
      int       y = _ty + (i & 1) * (i - 2);
      // Person is behind a bar
      if (m.data[y][x].attr->behavior == 0x80)
        {
          _tx = x;
          _ty = y;
          _tid = -1;
          _init();
          return;
        }
    }
}

void		Action::MoveTo::_init()
{
  Player	&p = _data.player();
  PathFinder	finder(_data.world()[p.getBank()][p.getMap()]);

  if (_tid != -1 && _tid >= _data.world()[p.getBank()][p.getMap()].nbPersons)
    {
      fprintf(stderr, "%d is not a valid person ID\n", _tid);
      _state = Action::ERROR;
      return;
    }
  _updateTargetPos();
  _pathi = 1;
  if (_path)
    {
      delete _path;
      _path = NULL;
    }
  _oldx = p.getX();
  _oldy = p.getY();
  if (p.getX() == _tx && p.getY() == _ty)
    {
      _state = Action::FINISHED;
      return;
    }
  _path = finder.search(p.getX(), p.getY(), _tx, _ty, _approx);
  if (!_path && _tid != -1)
    _searchBehindBar();
  _state = _path ? Action::RUNNING : Action::ERROR;
}

void		Action::MoveTo::_update()
{
  const OverWorld       &ow = _data.overWorld(0);
  World::Map::Node      *end = (*_path)[_path->size() - 1];
  Player	&p = _data.player();
  bool		moved = _oldx != ow.getDestX() || _oldy != ow.getDestY();

  if (_path && _pathi == _path->size() &&
      ow.getCurrX() == end->x && ow.getCurrY() == end->y)
    {
      _state = Action::FINISHED;
      delete _path;
      _path = NULL;
    }
  else if (_path && _pathi < _path->size() && (_pathi == 1 || moved) &&
           (ow.getDestX() != end->x || ow.getDestY() != end->y))
    {
      int	dx = (*_path)[_pathi]->x - ow.getDestX();
      int	dy = (*_path)[_pathi]->y - ow.getDestY();
      EKey	k;

      _releaseKeys();
      if (!dx)
	k = dy < 0 ? KEY_UP : KEY_DOWN;
      else
	k = dx < 0 ? KEY_LEFT : KEY_RIGHT;
      sdlSetButton(k, true);
      _pathi++;
    }
  if (ow.getDestX() == end->x && ow.getDestY() == end->y)
    _releaseKeys();
  _oldx = ow.getDestX();
  _oldy = ow.getDestY();
}
