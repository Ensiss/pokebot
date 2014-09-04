#include	"PokeScript.hh"

PokeScript::PokeScript(Data &data)
  : _data(data)
{
  _initInstructions();
}

PokeScript::~PokeScript()
{
}

void		PokeScript::_reset()
{
  while (_addrs.size())
    _addrs.pop();
  _ranges.clear();
  _addrs.push(_offset);
}

bool		PokeScript::_setupNextAddr()
{
  _ptr = NULL;
  while (!_ptr && _addrs.size())
    {
      uint16_t	i;

      _start = _addrs.front();
      _addrs.pop();
      for (i = 0; i < _ranges.size(); i++)
	if (_start >= _ranges[i].start && _start < _ranges[i].end)
	  break;
      if (i == _ranges.size())
	{
	  _ptr = (uint8_t *) gbaMem(_start);
	  _pc = _oldpc = 0;
	  return (true);
	}
    }
  return (false);
}

void		PokeScript::print(uint32_t ptr)
{
  uint8_t	id;

  _offset = ptr;
  _reset();
  while (_setupNextAddr())
    {
      printf("\n");
      do
	{
	  _oldpc = _pc;
	  id = _ptr[_pc++];
	  if (_inst[id])
	    (this->*_inst[id])();
	  else
	    _print("ERROR: Unknown opcode %#02x", id);
	}
      while (id != 0x02 && id != 0x03 && _inst[id]);
      _ranges.push_back(Range(_start, _start + _pc));
    }
  printf("\n");
}

void		PokeScript::printStd(uint8_t n)
{
  print(((uint32_t *) gbaMem(0x08160450))[n]);
}
