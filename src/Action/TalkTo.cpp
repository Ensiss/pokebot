#include	"Action/TalkTo.hh"

Action::TalkTo::TalkTo(uint8_t personId)
  : _pid(personId), _dir(0), _first(true)
{
}

Action::TalkTo::~TalkTo()
{
}

void		Action::TalkTo::_init()
{
  const OverWorld	*ows = _data.overWorlds();
  Player	&p = _data.player();
  World::Map    &m = _data.world()[p.getBank()][p.getMap()];
  uint16_t      tx, ty, px, py;
  bool          found = false;

  for (int i = 1; !found && i < 16 && (ows[i].getMap() || ows[i].getBank()); i++)
    {
      if (ows[i].getEventNb() == _pid)
        {
          tx = ows[i].getDestX();
          ty = ows[i].getDestY();
          found = true;
        }
    }
  for (int i = 0; !found && i < m.nbPersons; i++)
    {
      if (m.persons[i].evtNb == _pid)
        {
          tx = m.persons[i].x;
          ty = m.persons[i].y;
          found = true;
        }
    }
  px = p.getX();
  py = p.getY();
  if (abs(tx - px) + abs(ty - py) != 1)
    {
      _state = Action::ERROR;
      return;
    }
  _dir = (py < ty ? 1 : py > ty ? 2 :
          px > tx ? 3 : 4);
  if (ows[0].getDir() == _dir)
    _dir = 0;
}

void		Action::TalkTo::_update()
{
  EKey          dirKey[4] = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

  if (_first)
    {
      if (_dir)
        queue(new Action::PressButton(dirKey[_dir - 1]));
      queue(new Action::PressButton(KEY_BUTTON_A));
      _first = false;
    }
  else if (!_actions.size())
    {
      sdlSetButton(KEY_BUTTON_A, false);
      _state = Action::FINISHED;
    }
}
