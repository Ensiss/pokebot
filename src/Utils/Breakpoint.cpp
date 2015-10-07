#include	"Data.hh"
#include	"Script.hh"
#include        "VM.hh"

static Script   *_getScript()
{
  uint8_t       idx = gbaMem<uint8_t>(0x03005074);
  if (!idx)
    return (NULL);
  uint8_t       evt = Data::data->getOverWorld(idx).getEventNb();
  return (Script::getPerson(evt - 1));
}

void            debuggerBreakOnWrite(u32 address, u32 oldvalue, u32 value, int size, int t)
{
  Script        *sc = _getScript();

  // End of script execution
  if (oldvalue && !value)
    {
      VM::vm->updateAndCallback();
      if (sc)
        VM::vm->execCountNewVisits(sc);
      return;
    }
  if (!sc)
    return;
  sc->setExecuted();
  std::map<int, Script::Instruction *> &map = sc->getInstructions();

  for (std::map<int, Script::Instruction *>::iterator it = map.begin(); it != map.end(); it++)
    {
      Script::Instruction *instr = it->second;

      if (instr->next == oldvalue)
        {
          if (instr->cmd != 0x06 && instr->cmd != 0x07)
            break;
          bool res = instr->args[1] == value;
          if (instr->notVisited(res))
            instr->visit(res);
          break;
        }
    }
}
