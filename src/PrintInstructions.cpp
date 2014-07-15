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
  _inst[i = 0x16] = &PokeScript::_setvar;
  _inst[i = 0x17] = &PokeScript::_addvar;
  _inst[i = 0x18] = &PokeScript::_subvar;
  _inst[i = 0x19] = &PokeScript::_copyvar;
  _inst[i = 0x1A] = &PokeScript::_copyvarifnotzero;
  _inst[i = 0x21] = &PokeScript::_compare;
  _inst[i = 0x25] = &PokeScript::_special;
  _inst[i = 0x26] = &PokeScript::_special2;
  _inst[i = 0x27] = &PokeScript::_waitstate;
  _inst[i = 0x28] = &PokeScript::_pause;
  _inst[i = 0x29] = &PokeScript::_setflag;
  _inst[i = 0x2A] = &PokeScript::_clearflag;
  _inst[i = 0x2B] = &PokeScript::_checkflag;
  _inst[i = 0x2F] = &PokeScript::_sound;
  _inst[i = 0x4F] = &PokeScript::_applymovement;
  _inst[i = 0x51] = &PokeScript::_waitmovement;
  _inst[i = 0x53] = &PokeScript::_hidesprite;
  _inst[i = 0x5A] = &PokeScript::_faceplayer;
  _inst[i = 0x64] = &PokeScript::_moveoffscreen;
  _inst[i = 0x68] = &PokeScript::_closeonkeypress;
  _inst[i = 0x69] = &PokeScript::_lockall;
  _inst[i = 0x6A] = &PokeScript::_lock;
  _inst[i = 0x6B] = &PokeScript::_releaseall;
  _inst[i = 0x6C] = &PokeScript::_release;
  _inst[i = 0x6D] = &PokeScript::_waitkeypress;
  _inst[i = 0x7C] = &PokeScript::_checkattack;
  _inst[i = 0x7D] = &PokeScript::_bufferpokemon;
  _inst[++i] = &PokeScript::_bufferfirstpokemon;
  _inst[++i] = &PokeScript::_bufferpartypokemon;
  _inst[++i] = &PokeScript::_bufferitem;
  _inst[++i] = &PokeScript::_bufferdecoration;
  _inst[++i] = &PokeScript::_bufferattack;
  _inst[++i] = &PokeScript::_buffernumber;
  _inst[++i] = &PokeScript::_bufferstd;
  _inst[++i] = &PokeScript::_bufferstring;
  _inst[i = 0x9C] = &PokeScript::_doanimation;
  _inst[i = 0x9D] = &PokeScript::_setanimation;
  _inst[i = 0x9E] = &PokeScript::_checkanimation;
  _inst[i = 0xC7] = &PokeScript::_textcolor;
  _inst[i = 0xCA] = &PokeScript::_signmsg;
  _inst[i = 0xCB] = &PokeScript::_normalmsg;
}

void		PokeScript::_print(const char *s, ...)
{
  va_list	list;

  printf("%#08x: ", _start + _oldpc);
  for (int i = 0; i < 10; i++)
    {
      if (_oldpc + i < _pc || !_inst[_ptr[_oldpc]])
	printf("%02x ", _ptr[_oldpc + i]);
      else
	printf("   ");
    }
  va_start(list, s);
  vprintf(s, list);
  va_end(list);
  printf("\n");
}

const char	*PokeScript::_cmpOp(uint8_t b)
{
  return ("<\0\0==\0>\0\0<=\0>=\0!=\0?" + (b > 5 ? 18 : b * 3));
}

void		PokeScript::_if1()
{
  const char *op = _cmpOp(_readByte());
  uint32_t ptr = _readPointer();

  _addrs.push(ptr);
  _print("if %s goto %#08x", op, ptr);
}

void		PokeScript::_if2()
{
  const char *op = _cmpOp(_readByte());
  uint32_t ptr = _readPointer();

  _addrs.push(ptr);
  _print("if %s call %#08x", op, ptr);
}

void		PokeScript::_call()
{
  uint32_t ptr = _readPointer();

  _addrs.push(ptr);
  _print("call %#08x", ptr);
}

void		PokeScript::_goto()
{
  uint32_t ptr = _readPointer();

  _addrs.push(ptr);
  _print("goto %#08x", ptr);
}

void		PokeScript::_loadpointer()
{
  uint8_t	b = _readBank();
  uint32_t	ptr = _readDword();

  if (_ptr[_pc] == 0x09)
    {
      char		msg[1024];
      uint8_t		*addr = (uint8_t *) gbaMem(ptr);
      int		i;

      for (i = 0; i < 1024 && addr[i] != 0xFF; i++)
	msg[i] = pokeCharsetToAscii(addr[i]);
      msg[i] = '\0';
      _pc += 2;
      _print("msgbox %#02x \"%s\"", _ptr[_pc - 1], msg);
    }
  else
    _print("loadpointer %d %#08x", b, ptr);
}

void		PokeScript::_bufferstring()
{
  uint8_t	buff = _readBuffer();
  uint32_t	ptr = _readPointer();
  char		msg[1024];
  uint8_t	*addr = (uint8_t *) gbaMem(ptr);
  int		i;

  for (i = 0; i < 1024 && addr[i] != 0xFF; i++)
    msg[i] = pokeCharsetToAscii(addr[i]);
  msg[i] = '\0';
  _print("bufferstring %d \"%s\"", buff, msg);
}
