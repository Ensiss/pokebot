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
  saved = _states.front();
  _ctx = *saved;
  delete saved;
  _states.pop();
  return (true);
}

/**
   Counts how many key points would be visited if the script was executed

   The function simulates the execution of a script, and counts how many key
   points the script flow would visit.
   Key points represents the differents branches that the script can take. There
   is a key point for the success and one for the failure of the if1 (0x06) and
   the if2 (0x07) commands.

   @param script        The script to execute
   @return              The number of visited key points
 */
std::vector<VM::ChoicePts>      *VM::execCountNewVisits(Script &script)
{
  std::map<int, Script::Instruction *>  &instMap = script.getInstructions();
  std::vector<ChoicePts>                *cpts = new std::vector<ChoicePts>();
  uint32_t      oldpc;

  _ctx.update();
  _ctx.pc = script.getStartOffset();
  do {
    printf("\n\nLoaded context @0x%08x\n", _ctx.pc);
    while (_ctx.pc)
      {
        Script::Instruction *instr = instMap[_ctx.pc];
        oldpc = _ctx.pc = instr->next;
        if (_executers[instr->cmd])
          {
            printf("\t\t");
            (this->*_executers[instr->cmd])(instr);
          }
        if (instr->cmd == 0x06 || instr->cmd == 0x07)
          {
            if (instr->notVisited(oldpc != _ctx.pc))
              _ctx.cpts.pts++;
          }
        printf("%s\n", instr->str.c_str());
      }
    cpts->push_back(_ctx.cpts);
  } while (_restoreContext());
  return (cpts);
}

/**
   Executes a script

   @param script        The script to execute
 */
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
