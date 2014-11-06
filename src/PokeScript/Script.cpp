#include	"Script.hh"

Script::Script()
  : _data(*Action::data)
{
}

Script::~Script()
{
}

void		Script::_print(const char *s, ...)
{
  va_list	list;

  printf("%#08x: ", _start + _oldpc);
  for (int i = 0; i < 10; i++)
    {
      if (_oldpc + i < _pc || _cmds[_ptr[_oldpc]].format == "")
	printf("%02x ", _ptr[_oldpc + i]);
      else
	printf("   ");
    }
  va_start(list, s);
  vprintf(s, list);
  va_end(list);
  printf("\n");
}

void		Script::_reset()
{
  while (_addrs.size())
    _addrs.pop();
  _ranges.clear();
  _addrs.push(_offset);
}

bool		Script::_setupNextAddr()
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

void            Script::_getInstruction(Command &cmd)
{
  std::istringstream    iss(cmd.args);
  std::list<std::string>args{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
  std::size_t           next, i = 0;
  std::string           fmt, out = "";
  uint32_t              arg;
  char                  buff[64];

  do {
    next = cmd.format.find('%', i + 1);
    fmt = cmd.format.substr(i, next == std::string::npos ? next : next - i);
    if (!i)
      out += fmt;
    else
      {
        if (!args.size())
          {
            std::cerr << "WARNING: not enough arguments given (" << cmd.format << " / " << cmd.args << ")" << std::endl;
            arg = _readByte();
          }
        else
          {
            if (_readers.find(args.front()) != _readers.end())
              arg = (this->*_readers[args.front()])();
            else
              {
                std::cerr << "WARNING: unknown argument type: " << args.front() << std::endl;
                arg = _readByte();
              }
            args.pop_front();
          }
        snprintf(buff, sizeof(buff), fmt.c_str(), arg);
        out += std::string(buff);
      }
    i = next;
  } while (next != std::string::npos);
  std::cout << out << std::endl;
}

void		Script::print(uint32_t ptr)
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
	  if (_cmds[id].format != "")
	    _getInstruction(_cmds[id]);
	  else
	    _print("ERROR: Unknown opcode %#02x", id);
	}
      while (id != 0x02 && id != 0x03 && _cmds[id].format != "");
      _ranges.push_back(Range(_start, _start + _pc));
    }
  printf("\n");
}

void		Script::printStd(uint8_t n)
{
  print(((uint32_t *) gbaMem(0x08160450))[n]);
}
