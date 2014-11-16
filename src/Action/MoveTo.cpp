#include	"Action/MoveTo.hh"

Action::MoveTo::MoveTo(uint16_t x, uint16_t y)
  : _owInit(false), _tx(x), _ty(y), _tid(-1), _path(NULL)
{
  addListener("onFrame", &Action::MoveTo::_checkNPCMovement);
}

Action::MoveTo::MoveTo(uint8_t tid)
  : _owInit(false), _tx(0), _ty(0), _tid(tid), _path(NULL)
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

  for (int i = 1; i < 16 && (ows[i].getMap() || ows[i].getBank()); i++)
    {
      if (ows[i].getEventNb() == _tid)
        {
          _tx = ows[i].getDestX();
          _ty = ows[i].getDestY();
          return;
        }
    }
  for (int i = 0; i < m.nbPersons; i++)
    {
      if (m.persons[i].evtNb == _tid)
        {
          _tx = m.persons[i].x;
          _ty = m.persons[i].y;
          return;
        }
    }
}

void		Action::MoveTo::_init()
{
  Player	&p = _data.player();
  PathFinder	finder(_data.world()[p.getBank()][p.getMap()]);

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
  _path = finder.search(p.getX(), p.getY(), _tx, _ty, _tid != -1);
  _state = _path ? Action::RUNNING : Action::ERROR;
}

void		Action::MoveTo::_update()
{
  const OverWorld       &ow = _data.overWorld(0);
  Player	&p = _data.player();
  bool		moved = _oldx != p.getX() || _oldy != p.getY();
  World::Map::Node      *end = (*_path)[_path->size() - 1];

  if (_path && _pathi == _path->size() &&
      ow.getCurrX() == end->x && ow.getCurrY() == end->y)
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
