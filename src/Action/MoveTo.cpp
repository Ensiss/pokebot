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

  _listeners[signal]._cppListeners.push_back(l);
}

void		Action::MoveTo::_checkNPCMovement()
{
  const OverWorld       &p = _data.getOverWorld(0);
  const OverWorld	*ows = _data.getOverWorlds();

  // If a npc moved on the screen, recompute the path to avoid it
  for (int i = 1; _owInit && i < 16 && (ows[i].getMapId() || ows[i].getBankId()); i++)
    {
      if (ows[i].getBankId() == _data.getPlayer().getBankId() &&// If the overworld is in our map
	  ows[i].getMapId() == _data.getPlayer().getMapId() &&
	  (ows[i].getDestX() != _oldow[i].getDestX() ||		// If the overworld is moving
	   ows[i].getDestY() != _oldow[i].getDestY() ||
	   ows[i].getBankId() != _oldow[i].getBankId() ||		// Or if it's a new overworld
	   ows[i].getMapId() != _oldow[i].getMapId()))
	emit("onInit");
    }
  memcpy((void *) (_oldow + 1), (void *) (ows + 1), 15 * sizeof(OverWorld));
  _owInit = true;

  // If the bot was un-synchronized for some reason, reset it
  if (_path && _pathi > 0 && (abs(p.getDestX() - (*_path)[_pathi - 1]->getX()) > 0 && abs(p.getDestY() - (*_path)[_pathi - 1]->getY()) > 0))
      emit("onInit");
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

  const OverWorld	*ows = _data.getOverWorlds();
  Player	&p = _data.getPlayer();
  World::Map    &m = _data.getWorld()[p.getBankId()][p.getMapId()];
  const World::PersonEvt &pers = m.getPerson(_tid);

  _tx = pers.getX();
  _ty = pers.getY();
  for (int i = 1; i < 16 && (ows[i].getMapId() || ows[i].getBankId()); i++)
    {
      if (ows[i].getEventNb() == pers.getEventNb())
        {
          _tx = ows[i].getDestX();
          _ty = ows[i].getDestY();
          return;
        }
    }
}

void            Action::MoveTo::_searchBehindBar()
{
  Player	&p = _data.getPlayer();
  World::Map    &m = _data.getWorld()[p.getBankId()][p.getMapId()];

  for (int i = 0; i < 4; i++)
    {
      uint32_t  x = _tx + !(i & 1) * (i - 1);
      uint32_t  y = _ty + (i & 1) * (i - 2);
      // Person is behind a bar
      if (x >= 0 && y >= 0 && x < m.getWidth() && y < m.getHeight() && m.getNode(x, y).getBehavior() == 0x80)
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
  Player	&p = _data.getPlayer();
  PathFinder	finder(_data.getWorld()[p.getBankId()][p.getMapId()]);

  if (_tid != -1 && _tid >= _data.getWorld()[p.getBankId()][p.getMapId()].getNbPersons())
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
  const OverWorld       &ow = _data.getOverWorld(0);
  World::Map::Node      *end = (*_path)[_path->size() - 1];
  bool		moved = _oldx != ow.getDestX() || _oldy != ow.getDestY();

  if (_path && _pathi == _path->size() &&
      ow.getCurrX() == end->getX() && ow.getCurrY() == end->getY())
    {
      _state = Action::FINISHED;
      delete _path;
      _path = NULL;
    }
  else if (_path && _pathi < _path->size() && (_pathi == 1 || moved) &&
           (ow.getDestX() != end->getX() || ow.getDestY() != end->getY()))
    {
      int	dx = (*_path)[_pathi]->getX() - ow.getDestX();
      int	dy = (*_path)[_pathi]->getY() - ow.getDestY();
      EKey	k;

      _releaseKeys();
      if (!dx)
	k = dy < 0 ? KEY_UP : KEY_DOWN;
      else
	k = dx < 0 ? KEY_LEFT : KEY_RIGHT;
      sdlSetButton(k, true);
      _pathi++;
    }
  if (ow.getDestX() == end->getX() && ow.getDestY() == end->getY())
    _releaseKeys();
  _oldx = ow.getDestX();
  _oldy = ow.getDestY();
}
