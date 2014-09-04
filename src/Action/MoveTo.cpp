#include	"Action/MoveTo.hh"

Action::MoveTo::MoveTo(uint16_t x, uint16_t y)
  : _owInit(false), _tx(x), _ty(y), _path(NULL)
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

void		Action::MoveTo::_init()
{
  Player	&p = _data.player();
  PathFinder	finder(_data.world()[p.getBank()][p.getMap()]);

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
  _path = finder.search(p.getX(), p.getY(), _tx, _ty);
  _state = _path ? Action::RUNNING : Action::ERROR;
}

void		Action::MoveTo::_update()
{
  Player	&p = _data.player();
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
