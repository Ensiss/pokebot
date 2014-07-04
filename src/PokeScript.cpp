#include	"PokeScript.hh"

PokeScript::PokeScript(uint32_t ptr)
  : _offset(ptr)
{
  _initInstructions();
  reset();
}

PokeScript::~PokeScript()
{
}

void		PokeScript::reset()
{
  _ptr = (uint8_t *) gbaMem(_offset);
  _pc = 0;
  _oldpc = 0;
}

void		PokeScript::print()
{
  uint8_t	id;

  for (int i = 0; i < 80; i++)
    printf("%02x ", _ptr[i]);
  printf("\n");

  reset();
  do
    {
      _oldpc = _pc;
      id = _ptr[_pc++];
      if (_inst[id])
	(this->*_inst[id])();
      else
	_print("ERROR: Unknown opcode %#02x", id);
    }
  while (id != 0x02 && _inst[id]);
}
