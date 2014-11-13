#include        "VM.hh"

VM::VM()
  : _ctx()
{
}

VM::~VM()
{
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
