#include        "VM.hh"

std::function<bool(uint32_t, uint32_t)> VM::_cmpOp[6] = {
  std::less<uint32_t>(),
  std::equal_to<uint32_t>(),
  std::greater<uint32_t>(),
  std::less_equal<uint32_t>(),
  std::greater_equal<uint32_t>(),
  std::not_equal_to<uint32_t>()
};

VM::Executer VM::_executers[0xD6] = {
  /* 00 */ NULL,
  /* 01 */ NULL,
  /* 02 */ NULL,
  /* 03 */ &VM::_return,
  /* 04 */ &VM::_call,
  /* 05 */ &VM::_goto,
  /* 06 */ &VM::_if1,
  /* 07 */ &VM::_if2,
  /* 08 */ NULL,
  /* 09 */ NULL,
  /* 0A */ NULL,
  /* 0B */ NULL,
  /* 0C */ NULL,
  /* 0D */ NULL,
  /* 0E */ NULL,
  /* 0F */ &VM::_loadpointer,
  /* 10 */ &VM::_setbyte2,
  /* 11 */ NULL,
  /* 12 */ &VM::_loadbytefrompointer,
  /* 13 */ NULL,
  /* 14 */ &VM::_copyscriptbanks,
  /* 15 */ NULL,
  /* 16 */ &VM::_setvar,
  /* 17 */ &VM::_addvar,
  /* 18 */ &VM::_subvar,
  /* 19 */ &VM::_copyvar,
  /* 1A */ &VM::_copyvarifnotzero,
  /* 1B */ &VM::_comparebanks,
  /* 1C */ &VM::_comparebanktobyte,
  /* 1D */ &VM::_comparebanktofarbyte,
  /* 1E */ &VM::_comparefarbytetobank,
  /* 1F */ &VM::_comparefarbytetobyte,
  /* 20 */ &VM::_comparefarbytes,
  /* 21 */ &VM::_compare,
  /* 22 */ &VM::_comparevars,
  /* 23 */ NULL,
  /* 24 */ NULL,
  /* 25 */ NULL,
  /* 26 */ NULL,
  /* 27 */ NULL,
  /* 28 */ NULL,
  /* 29 */ &VM::_setflag,
  /* 2A */ &VM::_clearflag,
  /* 2B */ &VM::_checkflag,
  /* 2C */ NULL,
  /* 2D */ NULL,
  /* 2E */ &VM::_resetvars,
  /* 2F */ NULL,
  /* 30 */ NULL,
  /* 31 */ NULL,
  /* 32 */ NULL,
  /* 33 */ NULL,
  /* 34 */ NULL,
  /* 35 */ NULL,
  /* 36 */ NULL,
  /* 37 */ NULL,
  /* 38 */ NULL,
  /* 39 */ NULL,
  /* 3A */ NULL,
  /* 3B */ NULL,
  /* 3C */ NULL,
  /* 3D */ NULL,
  /* 3E */ NULL,
  /* 3F */ NULL,
  /* 40 */ NULL,
  /* 41 */ NULL,
  /* 42 */ NULL,
  /* 43 */ NULL,
  /* 44 */ NULL,
  /* 45 */ NULL,
  /* 46 */ NULL,
  /* 47 */ NULL,
  /* 48 */ NULL,
  /* 49 */ NULL,
  /* 4A */ NULL,
  /* 4B */ NULL,
  /* 4C */ NULL,
  /* 4D */ NULL,
  /* 4E */ NULL,
  /* 4F */ NULL,
  /* 50 */ NULL,
  /* 51 */ NULL,
  /* 52 */ NULL,
  /* 53 */ NULL,
  /* 54 */ NULL,
  /* 55 */ NULL,
  /* 56 */ NULL,
  /* 57 */ NULL,
  /* 58 */ NULL,
  /* 59 */ NULL,
  /* 5A */ NULL,
  /* 5B */ NULL,
  /* 5C */ NULL,
  /* 5D */ NULL,
  /* 5E */ NULL,
  /* 5F */ NULL,
  /* 60 */ NULL,
  /* 61 */ NULL,
  /* 62 */ NULL,
  /* 63 */ NULL,
  /* 64 */ NULL,
  /* 65 */ NULL,
  /* 66 */ NULL,
  /* 67 */ NULL,
  /* 68 */ NULL,
  /* 69 */ NULL,
  /* 6A */ NULL,
  /* 6B */ NULL,
  /* 6C */ NULL,
  /* 6D */ NULL,
  /* 6E */ NULL,
  /* 6F */ NULL,
  /* 70 */ NULL,
  /* 71 */ NULL,
  /* 72 */ NULL,
  /* 73 */ NULL,
  /* 74 */ NULL,
  /* 75 */ NULL,
  /* 76 */ NULL,
  /* 77 */ NULL,
  /* 78 */ NULL,
  /* 79 */ NULL,
  /* 7A */ NULL,
  /* 7B */ NULL,
  /* 7C */ NULL,
  /* 7D */ NULL,
  /* 7E */ NULL,
  /* 7F */ NULL,
  /* 80 */ NULL,
  /* 81 */ NULL,
  /* 82 */ NULL,
  /* 83 */ NULL,
  /* 84 */ NULL,
  /* 85 */ NULL,
  /* 86 */ NULL,
  /* 87 */ NULL,
  /* 88 */ NULL,
  /* 89 */ NULL,
  /* 8A */ NULL,
  /* 8B */ NULL,
  /* 8C */ NULL,
  /* 8D */ NULL,
  /* 8E */ NULL,
  /* 8F */ NULL,
  /* 90 */ NULL,
  /* 91 */ NULL,
  /* 92 */ NULL,
  /* 93 */ NULL,
  /* 94 */ NULL,
  /* 95 */ NULL,
  /* 96 */ NULL,
  /* 97 */ NULL,
  /* 98 */ NULL,
  /* 99 */ NULL,
  /* 9A */ NULL,
  /* 9B */ NULL,
  /* 9C */ NULL,
  /* 9D */ NULL,
  /* 9E */ NULL,
  /* 9F */ NULL,
  /* A0 */ NULL,
  /* A1 */ NULL,
  /* A2 */ NULL,
  /* A3 */ NULL,
  /* A4 */ NULL,
  /* A5 */ NULL,
  /* A6 */ NULL,
  /* A7 */ NULL,
  /* A8 */ NULL,
  /* A9 */ NULL,
  /* AA */ NULL,
  /* AB */ NULL,
  /* AC */ NULL,
  /* AD */ NULL,
  /* AE */ NULL,
  /* AF */ NULL,
  /* B0 */ NULL,
  /* B1 */ NULL,
  /* B2 */ NULL,
  /* B3 */ NULL,
  /* B4 */ NULL,
  /* B5 */ NULL,
  /* B6 */ NULL,
  /* B7 */ NULL,
  /* B8 */ NULL,
  /* B9 */ NULL,
  /* BA */ NULL,
  /* BB */ NULL,
  /* BC */ NULL,
  /* BD */ NULL,
  /* BE */ NULL,
  /* BF */ NULL,
  /* C0 */ NULL,
  /* C1 */ NULL,
  /* C2 */ NULL,
  /* C3 */ NULL,
  /* C4 */ NULL,
  /* C5 */ NULL,
  /* C6 */ NULL,
  /* C7 */ NULL,
  /* C8 */ NULL,
  /* C9 */ NULL,
  /* CA */ NULL,
  /* CB */ NULL,
  /* CC */ NULL,
  /* CD */ NULL,
  /* CE */ NULL,
  /* CF */ NULL,
  /* D0 */ NULL,
  /* D1 */ NULL,
  /* D2 */ NULL,
  /* D3 */ NULL,
  /* D4 */ NULL,
  /* D5 */ NULL
};

void            VM::_return(Script::Instruction *instr)
{
  if (!_ctx.stack.size())
    {
      std::cerr << "VM error: cannot return (empty stack)" << std::endl;
      return;
    }
  _ctx.pc = _ctx.popStack();
}

void            VM::_call(Script::Instruction *instr)
{
  _ctx.pushStack(instr->next);
  _ctx.pc = instr->args[0];
}

void            VM::_goto(Script::Instruction *instr)
{
  _ctx.pc = instr->args[0];
}

void            VM::_if1(Script::Instruction *instr)
{
  if (instr->args[0] > 5)
    {
      std::cerr << "No such operator: " << instr->args[0] << std::endl;
      return;
    }
  if (_cmpOp[instr->args[0]](_ctx.cmp1, _ctx.cmp2))
    _ctx.pc = instr->args[1];
}

void            VM::_if2(Script::Instruction *instr)
{
  if (instr->args[0] > 5)
    {
      std::cerr << "No such operator: " << instr->args[0] << std::endl;
      return;
    }
  if (_cmpOp[instr->args[0]](_ctx.cmp1, _ctx.cmp2))
    {
      _ctx.pushStack(instr->next);
      _ctx.pc = instr->args[1];
    }
}
