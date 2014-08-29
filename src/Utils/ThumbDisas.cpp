#include	"ThumbDisas.hh"

ThumbDisas::ThumbDisas(uint8_t instr)
  : _instr(instr),
    _offset(*((uint32_t *) gbaMem(0x0815F9B4 + (instr << 2))) & ~1)
{
  _reset();
}

ThumbDisas::~ThumbDisas()
{
}

void		ThumbDisas::_reset()
{
  while (_addrs.size())
    _addrs.pop();
  _ranges.clear();
  _addrs.push(_offset);
}

bool		ThumbDisas::_setupNextAddr()
{
  while (_addrs.size())
    {
      uint16_t	i;

      _start = _addrs.front();
      _addrs.pop();
      for (i = 0; i < _ranges.size(); i++)
	if (_start >= _ranges[i].start && _start < _ranges[i].end)
	  break;
      if (i == _ranges.size())
	{
	  _pc = _oldpc = _start;
	  return (true);
	}
    }
  return (false);
}

void		ThumbDisas::print()
{
  char		ins[128];
  uint32_t	jump;
  char		*ptr;

  _reset();
  while (_setupNextAddr())
    {
      printf("\n");
      do
	{
	  _oldpc = _pc;
	  _pc += disThumb(_pc, ins, 0);
	  if (_oldpc == _start || 1)
	    printf("0x%08x: ", _oldpc);
	  else
	    for (int i = 0; i < 12; i++)
	      printf(" ");
	  printf("%s\n", ins);
	  if (ins[0] == 'b' && (ptr = strchr(ins, '$')))
	    {
	      sscanf(ptr + 1, "%x", &jump);
	      if (jump >= 0x08000000)
		{
		  _addrs.push(jump);
		  if (ins[1] == ' ')
		    break;
		}
	    }
	}
      while (ins[0] != 'b' || ins[1] != 'x');
      _ranges.push_back(Range(_start, _pc));
    }
  printf("\n");
}
