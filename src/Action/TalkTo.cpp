#include	"Action/TalkTo.hh"

/**
   Talk to a person

   This action walks to a person, turns towards it and then speaks to it.
   You can specify what the bot will answer in case there's a multichoice box or
   a yes/no choice.

   @param personId      Index of the person in the person array
   @param choices       List of answers (returned by VM::execCountNewVisits)
 */
Action::TalkTo::TalkTo(uint8_t personId, VM::ChoicePts *choices)
  : _pid(personId), _dir(0), _oldNext(0), _script(NULL), _choices(choices), _choiceId(0)
{
}

Action::TalkTo::~TalkTo()
{
}

void            Action::TalkTo::_turnToOW()
{
  const OverWorld	*ows = _data.overWorlds();
  EKey          dirKey[4] = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};
  Player	&p = _data.player();
  World::Map    &m = _data.world()[p.getBank()][p.getMap()];
  uint16_t      tx, ty, px, py;

  tx = m.persons[_pid].x;
  ty = m.persons[_pid].y;
  for (int i = 1; i < 16 && (ows[i].getMap() || ows[i].getBank()); i++)
    {
      if (ows[i].getEventNb() == m.persons[_pid].evtNb)
        {
          tx = ows[i].getDestX();
          ty = ows[i].getDestY();
          break;
        }
    }
  px = p.getX();
  py = p.getY();
  _dir = (py < ty ? 1 : py > ty ? 2 : px > tx ? 3 : 4);
  if (ows[0].getDir() != _dir)
    queue(new Action::PressButton(dirKey[_dir - 1]));
}

void		Action::TalkTo::_init()
{
  World::Map    &m = _data.world()[_data.player().getBank()][_data.player().getMap()];

  if (_pid >= m.nbPersons)
    {
      fprintf(stderr, "%d is not a valid person ID\n", _pid);
      _state = Action::ERROR;
      return;
    }
  queue(new Action::MoveTo(_pid));
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
    if ((instr = _searchCmd(Script::getStd(i)->getInstructions(), next)))
      return (instr);
  return (NULL);
}

void            Action::TalkTo::_handleMultiChoice()
{
  if (!_choices || _choiceId >= _choices->choices.size())
    return;
  int           res = _choices->choices[_choiceId];
  queue(new Action::Wait(5));
  if (res == 0x7F)
    queue(new Action::PressButton(KEY_BUTTON_B));
  else
    {
      while (--res >= 0)
        queue(new Action::PressButton(KEY_DOWN));
      queue(new Action::PressButton(KEY_BUTTON_A));
    }
  _choiceId++;
}

void            Action::TalkTo::_handleYesNo()
{
  if (!_choices || _choiceId >= _choices->choices.size())
    return;
  int           res = _choices->choices[_choiceId];
  queue(new Action::Wait(5));
  queue(new Action::PressButton(res ? KEY_BUTTON_A : KEY_BUTTON_B));
  _choiceId++;
}

void		Action::TalkTo::_update()
{
  if (!getCounter())
    {
      _turnToOW();
      queue(new Action::PressButton(KEY_BUTTON_A));
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
      if (!_script)
        _script = Script::getPerson(_pid);
      Script::Instruction       *instr = _getCurrentCmd();

      if (instr)
        {
          if (instr->cmd == 0x66) // waitmsg
            {
              // Pointer to remaining string to display
              uint32_t      ptr = *((uint32_t *) gbaMem(0x02020034));
              // Last character displayed
              uint8_t       chr = ((uint8_t *) gbaMem(ptr))[-1];
              if (chr == 0xFA || chr == 0xFB)
                queue(new Action::PressButton(KEY_BUTTON_A));
            }
          else if (instr->cmd == 0x6D || instr->cmd == 0x68) // waitkeypress/closeonkeypress
            {
              if (_oldNext != instr->next)
                {
                  queue(new Action::PressButton(KEY_BUTTON_A));
                  _oldNext = instr->next;
                }
            }
          else if (instr->cmd == 0x6F) // multichoice
            _handleMultiChoice();
          else if (instr->cmd == 0x6E) // yesnobox
            _handleYesNo();
        }
    }
}
