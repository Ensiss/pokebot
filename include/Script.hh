#ifndef		__SCRIPT_HH__
#define		__SCRIPT_HH__

#include        <sstream>
#include        <iterator>
#include        <iostream>
#include	<string>
#include	<cstdio>
#include	<stdarg.h>
#include	<stdint.h>
#include	<vector>
#include	<list>
#include	<queue>
#include        <map>

#include	"Action/AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"Data.hh"

class		Script
{
private:
  struct	Range
  {
    Range(uint32_t x = 0, uint32_t y = 0)
      : start(x), end(y) {}
    uint32_t	start;
    uint32_t	end;
  };

public:
  typedef       std::vector<uint32_t>           Args;
  typedef       std::vector<std::string>        TypeList;
  typedef       uint32_t(Script::*ParamReader)();

  struct        Instruction
  {
    uint32_t    offset;
    uint8_t     *bytecode;
    uint8_t     length;
    uint32_t    next;
    uint8_t     cmd;
    std::string str;
    Args        args;
    TypeList    types;
    uint8_t     toVisit; // bit field storing what parts of the branch was visited

    Instruction(uint32_t p_off, uint8_t *p_mem)
      : offset(p_off), bytecode(p_mem + p_off), length(1), cmd(*bytecode), str(""),
        toVisit((cmd == 0x06 || cmd == 0x07) * 3)
    {}
    bool        notVisited(bool result)
    { return (toVisit & (1 << result)); }
    void        visit(bool result)
    { toVisit &= ~(1 << result); }
    void        print()
    {
      printf("%#08x: ", offset);
      for (int i = 0; i < 10; i++)
        printf(i < length ? "%02x " : "   ", bytecode[i]);
      printf("%s\n", str.c_str());
    }
    void        setLength(uint8_t len)
    {
      length = len;
      next = (cmd == 0x02 || cmd == 0x03 ? 0 : offset + length);
    }
  };

  struct        Command
  {
    Command(std::initializer_list<std::string> il)
      : format(il.size() ? *(il.begin()) : "UNKNOWN"),
        args(il.size() > 1 ? *(il.begin() + 1) : ""),
        hook(NULL)
    {}
    Command(const std::string &fmt, const std::string &params = "", void (Script::*func)(Instruction *) = NULL)
      : format(fmt), args(params), hook(func)
    {}

    std::string format;
    std::string args;
    void        (Script::*hook)(Instruction *);
  };

public:
  Script();
  ~Script();

public:
  void		load(uint32_t ptr);
  void		loadStd(uint8_t n);
  void		print();

public:
  std::map<int, Instruction *>  &getInstructions() { return (_instructions); }
  uint32_t                      getStartOffset() { return (_offset); }

private:
  void          _subPrint(uint32_t ptr);
  void		_reset();
  bool		_setupNextAddr();
  void          _getInstruction(Command &cmd);

private:
  uint32_t	_readByte();
  uint32_t	_readWord();
  uint32_t	_readDword();

private:
  void          _loadpointer(Instruction *instr);
  void          _bufferstring(Instruction *instr);
  void          _preparemsg(Instruction *instr);
  void          _checkattack(Instruction *instr);
  void          _branch(Instruction *instr);

private:
  Data			&_data;
  uint32_t		_offset;
  uint32_t		_start;
  uint8_t		*_ptr;
  uint32_t		_pc;
  uint32_t		_oldpc;
  std::vector<Range>	_ranges;
  std::queue<uint32_t>	_addrs;
  std::map<int, Instruction *>  _instructions;

  static Command	_cmds[0xD6];
  static std::map<std::string, ParamReader>     _readers;
};

#endif
