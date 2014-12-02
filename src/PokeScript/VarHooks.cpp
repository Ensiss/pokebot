#include        "Script.hh"

#define         ARGNO(x)        (1 << (x))

std::map<uint16_t, std::vector<Script *> >      Script::_hookList;

std::map<uint8_t, uint8_t>      Script::_cmdHooks = {
  {0x19, ARGNO(1)},
  {0x1A, ARGNO(1)},
  {0x21, ARGNO(0)},
  {0x22, ARGNO(0) | ARGNO(1)},
  {0x2B, ARGNO(0)}
};

void            Script::_addHook(Instruction *instr)
{
  std::map<uint8_t, uint8_t>::iterator it = _cmdHooks.find(instr->cmd);

  if (it == _cmdHooks.end())
    return;
  for (uint8_t i = 0; i < instr->args.size(); i++)
    {
      if (it->second & (1 << i) &&
          (VM_IS_FLAG(instr->args[i]) || VM_IS_VAR(instr->args[i])))
        {
          std::vector<Script *> &list = _hookList[instr->args[i]];
          if (std::find(list.begin(), list.end(), this) == list.end())
            list.push_back(this);
        }
    }
}
