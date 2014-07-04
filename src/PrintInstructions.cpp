#include	"PokeScript.hh"

PokeScript::Instruction	PokeScript::_inst[0xD6] = { NULL };

void		PokeScript::_initInstructions()
{
  int		i;

  if (_inst[0])
    return;
  for (i = 0; i < 0xD6; i++)
    _inst[i] = NULL;
  _inst[i = 0x0] = &PokeScript::_nop;
  _inst[++i] = &PokeScript::_nop;
  _inst[++i] = &PokeScript::_end;
  _inst[++i] = &PokeScript::_return;
  _inst[++i] = &PokeScript::_call;
  _inst[++i] = &PokeScript::_goto;
  _inst[++i] = &PokeScript::_if1;
  _inst[++i] = &PokeScript::_if2;
  _inst[++i] = &PokeScript::_gotostd;
  _inst[++i] = &PokeScript::_callstd;
  _inst[++i] = &PokeScript::_gotostdif;
  _inst[++i] = &PokeScript::_callstdif;
  _inst[i = 0x0F] = &PokeScript::_loadpointer;
  _inst[i = 0x21] = &PokeScript::_compare;
  _inst[i = 0x2B] = &PokeScript::_checkflag;
  _inst[i = 0x2F] = &PokeScript::_sound;
  _inst[i = 0x4F] = &PokeScript::_applymovement;
  _inst[i = 0x51] = &PokeScript::_waitmovement;
  _inst[i = 0x68] = &PokeScript::_closeonkeypress;
  _inst[i = 0x6A] = &PokeScript::_lock;
}

void		PokeScript::_print(const char *s, ...)
{
  va_list	list;

  printf("%#08x: ", _offset + _oldpc);
  va_start(list, s);
  vprintf(s, list);
  va_end(list);
  printf("\n");
}
