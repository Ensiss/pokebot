#include	"Action/TalkTo.hh"

Action::TalkTo::TalkTo(uint8_t personId)
  : _pid(personId), _dir(0), _script(NULL)
{
}

Action::TalkTo::~TalkTo()
{
  if (_script)
    delete _script;
}

void            Action::TalkTo::_turnToOW()
{
  const OverWorld	*ows = _data.overWorlds();
  EKey          dirKey[4] = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};
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
  _dir = (py < ty ? 1 : py > ty ? 2 : px > tx ? 3 : 4);
  if (ows[0].getDir() != _dir)
    queue(new Action::PressButton(dirKey[_dir - 1]));
}

void		Action::TalkTo::_init()
{
  queue(new Action::MoveTo(_pid));
}

bool            Action::TalkTo::_loadScript()
{
  uint8_t       id = *((uint8_t *) gbaMem(0x03005074));
  uint8_t       evtNb = _data.overWorld(id).getEventNb();
  World::Map    &m = _data.world()[_data.player().getBank()][_data.player().getMap()];

  for (int i = 0; i < m.nbPersons; i++)
    {
      if (m.persons[i].evtNb == evtNb)
        {
          _script = new Script();
          _script->load(m.persons[i].scriptPtr);
          return (true);
        }
    }
  _state = Action::FINISHED;
  return (false);
}

Script::Instruction     *Action::TalkTo::_searchCmd(std::map<int, Script::Instruction *> &map, uint32_t next)
{
  for (std::map<int, Script::Instruction *>::iterator it = map.begin(); it != map.end(); it++)
    {
      Script::Instruction *instr = (*it).second;

      if (instr->next == next)
        return (instr);
    }
  return (NULL);
}

Script::Instruction     *Action::TalkTo::_getCurrentCmd()
{
  uint32_t              next = *((uint32_t *) gbaMem(0x03000EB8));
  Script::Instruction   *instr;

  if ((instr = _searchCmd(_script->getInstructions(), next)))
    return (instr);
  for (int i = 0; i < 10; i++)
    if ((instr = _searchCmd(Script::getStd(i).getInstructions(), next)))
      return (instr);
  return (NULL);
}

void		Action::TalkTo::_update()
{
  if (!_getCounter())
    {
      _turnToOW();
      queue(new Action::PressButton(KEY_BUTTON_A));
    }
  else
    {
      // if no script is running
      if (!*((uint8_t *) gbaMem(0x03000F9C)))
        {
          _state = Action::FINISHED;
          return;
        }
      if (!_script && !_loadScript())
        return;
      Script::Instruction       *instr = _getCurrentCmd();
      // if a message box is being drawn or waiting for key press
      if (instr && (instr->cmd == 0x66 || instr->cmd == 0x6D))
        queue(new Action::PressButton(KEY_BUTTON_A));
    }
}
