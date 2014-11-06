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
  typedef       std::vector<int>        Args;
  typedef       uint32_t(Script::*ParamReader)();

  struct        Instruction
  {
    uint32_t    offset;
    uint8_t     *bytecode;
    uint8_t     cmd;
    Args        args;
    std::string str;
  };

  struct        Command
  {
    Command(std::initializer_list<std::string> il)
      : format(il.size() ? *(il.begin()) : "UNKNOWN"),
        args(il.size() > 1 ? *(il.begin() + 1) : ""),
        processor(NULL)
    {}
    Command(const std::string &fmt, const std::string &params = "", void (*func)() = NULL)
      : format(fmt), args(params), processor(func)
    {}

    std::string format;
    std::string args;
    void        (*processor)();
  };

public:
  Script();
  ~Script();

public:
  void		print(uint32_t ptr);
  void		printStd(uint8_t n);

private:
  void		_reset();
  bool		_setupNextAddr();
  void          _getInstruction(Command &cmd);
  void		_print(const char *s, ...);

private:
  uint32_t	_readByte();
  uint32_t	_readWord();
  uint32_t	_readDword();
  uint32_t	_readPointer();
  uint32_t	_readVariable();
  uint32_t	_readFlag();
  uint32_t	_readBank();
  uint32_t	_readBuffer();
  uint32_t	_readHiddenVar();
  uint32_t	_readPtrOrBank();
  uint32_t	_readPtrOrBank0();
  uint32_t	_readFlagOrVar();
  uint32_t	_readWordOrVar();
  uint32_t	_readByteOrVar();

private:
  Data			&_data;
  uint32_t		_offset;
  uint32_t		_start;
  uint8_t		*_ptr;
  uint32_t		_pc;
  uint32_t		_oldpc;
  std::vector<Range>	_ranges;
  std::queue<uint32_t>	_addrs;
  static Command	_cmds[0xD6];
  static std::map<std::string, ParamReader>     _readers;
};

#endif
