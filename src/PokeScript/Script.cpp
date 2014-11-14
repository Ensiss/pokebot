#include	"Script.hh"

Script::Script()
  : _data(*Action::data)
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
  _instructions[instr->offset] = instr;
}

void            Script::_recCondTree(CtrlPoint &tree, uint32_t pc, int indent)
{
  while (pc)
    {
      Instruction *instr = _instructions[pc];
      if (instr->cmd == 0x06 || instr->cmd == 0x07)
        _recCondTree(tree.addChild(pc), instr->args[1], indent + 1);
      else if (instr->cmd == 0x04 || instr->cmd == 0x05)
        _recCondTree(tree, instr->args[0], indent);
      pc = instr->next;
    }
}

void            Script::_searchConditionTree()
{
  _recCondTree(_ctrlPt, _offset, 0);
}

void            Script::_recPrintCondTree(CtrlPoint &pt, int indent)
{
  if (pt.addr)
    {
      Instruction *instr = _instructions[pt.addr];
      printf("0x%08x: ", pt.addr);
      for (int i = 0; i < indent; i++)
        printf("  ");
      printf("%s\n", instr->str.c_str());
    }
  for (std::list<CtrlPoint>::iterator it = pt.children.begin(); it != pt.children.end(); it++)
    _recPrintCondTree(*it, indent + 1);
}

void            Script::printConditionTree()
{
  _recPrintCondTree(_ctrlPt, -1);
}

void		Script::load(uint32_t ptr)
{
  uint8_t	id;

  _offset = ptr;
  _reset();
  if (!ptr)
    {
      _addrs.pop();
      return;
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
  _searchConditionTree();
}

void		Script::loadStd(uint8_t n)
{
  load(((uint32_t *) gbaMem(0x08160450))[n]);
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
      if (_cmds[instr->cmd].hook == &Script::_branch)
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
