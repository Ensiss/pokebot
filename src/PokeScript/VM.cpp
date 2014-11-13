#include        "VM.hh"

VM::VM()
  : _ctx()
{
}

VM::~VM()
{
}

bool            VM::_restoreContext()
{
  Context       *saved;

  if (!_states.size())
    return (false);
  saved = _states.top();
  _ctx = *saved;
  delete saved;
  _states.pop();
  return (true);
}

void            VM::exec(Script &script)
{
  std::map<int, Script::Instruction *> &instMap = script.getInstructions();

  _ctx.clearStack();
  _ctx.pc = script.getStartOffset();
  while (_ctx.pc)
    {
      Script::Instruction *instr = instMap[_ctx.pc];
      _ctx.pc = instr->next;
      if (_executers[instr->cmd])
        {
          printf("\t\t");
          (this->*_executers[instr->cmd])(instr);
        }
      printf("%s\n", instr->str.c_str());
    }
}
