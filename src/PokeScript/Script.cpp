#include	"Script.hh"
#include        "VM.hh"

std::map<Script::Identifier, Script *>  Script::_cache;
std::map<Script::Identifier, Script *>  Script::_keyScripts;

Script::Script(uint8_t bank, uint8_t map, uint8_t id, ScriptType type)
  : _data(*Data::data), _id(bank, map, id, type), _executed(false)
{
}

Script::~Script()
{
}

void		Script::_reset()
{
  while (_addrs.size())
    _addrs.pop();
  _ranges.clear();
  _addrs.push(_offset);
  _instructions.clear();
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
  std::string           fmt = "";
  uint32_t              arg;
  char                  buff[64];
  Instruction           *instr = new Instruction(_start + _pc - 1, _ptr - _start);

  do {
    next = cmd.format.find('%', i + 1);
    fmt = cmd.format.substr(i, next == std::string::npos ? next : next - i);
    if (!i)
      instr->str += fmt;
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
              {
                instr->types.push_back(args.front());
                arg = (this->*_readers[args.front()])();
              }
            else
              {
                std::cerr << "WARNING: unknown argument type: " << args.front() << std::endl;
                arg = _readByte();
              }
            args.pop_front();
          }
        instr->args.push_back(arg);
        snprintf(buff, sizeof(buff), fmt.c_str(), arg);
        instr->str += std::string(buff);
      }
    i = next;
  } while (next != std::string::npos);
  if (cmd.hook)
    (this->*cmd.hook)(instr);
  instr->setLength(_start + _pc - instr->offset);
  _addHook(instr);
  _instructions[instr->offset] = instr;
}

Script		&Script::load(uint32_t ptr)
{
  uint8_t	id;

  _offset = ptr;
  _reset();
  if (!ptr)
    {
      _addrs.pop();
      return (*this);
    }
  while (_setupNextAddr())
    {
      do
	{
	  _oldpc = _pc;
	  id = _ptr[_pc++];
	  if (_cmds[id].format != "")
            _getInstruction(_cmds[id]);
	  else
	    printf("ERROR: Unknown opcode %#02x", id);
	}
      while (id != 0x02 && id != 0x03 && _cmds[id].format != "");
      _ranges.push_back(Range(_start, _start + _pc));
    }
  if (_id.type != STD)
    VM::vm->execCountNewVisits(this);
  return (*this);
}

Script		&Script::loadStd(uint8_t n)
{
  load(((uint32_t *) gbaMem(0x08160450))[n]);
  return (*this);
}

void            Script::_subPrint(uint32_t ptr)
{
  Instruction   *instr;
  uint32_t      beg = 0;

  std::cout << std::endl;
  while (_instructions.find(ptr) != _instructions.end())
    {
      instr = _instructions[ptr];
      if (!beg)
        beg = instr->offset;
      instr->print();
      if (_cmds[instr->cmd].hook == &Script::_branch ||
          _cmds[instr->cmd].hook == &Script::_if)
        _addrs.push(instr->args[instr->args.size() - 1]);
      ptr = instr->next;
    }
  _ranges.push_back(Range(beg, instr->offset + instr->length));
}

void            Script::print()
{
  _ranges.clear();
  while (_addrs.size())
    _addrs.pop();

  _addrs.push(_offset);
  while (_addrs.size())
    {
      uint32_t  addr = _addrs.front();
      _addrs.pop();
      for (std::vector<Range>::iterator it = _ranges.begin(); addr && it != _ranges.end(); it++)
        {
          if (addr >= (*it).start && addr < (*it).end)
            addr = 0;
        }
      if (addr)
        _subPrint(addr);
    }
  std::cout << std::endl;
}

Script          *Script::_getScript(uint8_t bank, uint8_t map, uint8_t id, ScriptType type)
{
  std::map<Identifier, Script *>::iterator it;
  Script        *sc = NULL;

  if (type < STD && !bank && !map)
    {
      bank = Data::data->player().getBank();
      map = Data::data->player().getMap();
    }
  it = _cache.find(Identifier(bank, map, id, type));
  if (it != _cache.end())
    return (it->second);

  if (type == STD)
    {
      if (id > 9)
        return (NULL);
      sc = new Script(bank, map, id, type);
      sc->loadStd(id);
      _cache[Identifier(bank, map, id, type)] = sc;
      return (sc);
    }

  World::Map    &m = Data::data->world()[bank][map];

  if ((type == PERSON && id < m.getNbPersons()) ||
      (type == SIGN && id < m.getNbSigns()) ||
      (type == SCRIPT && id < m.getNbScripts()))
    {
      sc = new Script(bank, map, id, type);
      sc->load(type == PERSON ? m.getPerson(id).getScript() :
               type == SIGN ? m.getSign(id).getScript() :
               m.getScript(id).getScript());
      _cache[Identifier(bank, map, id, type)] = sc;
      return (sc);
    }
  fprintf(stderr, "Error: cannot find scriptable %d of type %d in map [%d, %d]\n", id, type, bank, map);
  return (sc);
}
