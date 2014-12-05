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

#define         VM_FLAGS        0x900
#define         VM_VARS         0x100
#define         VM_TEMP         0x1F
#define         VM_BANKS        0x04
#define         VM_BUFF         0x03
#define         VM_VAR_OFFSET   0x4000
#define         VM_TEMP_OFFSET  0x8000

#define         VM_LASTRESULT   0x800D

#define         VM_IS_FLAG(x)   ((x) < VM_FLAGS)
#define         VM_IS_BANK(x)   ((x) < VM_BANKS)
#define         VM_IS_VAR(x)    ((x) >= VM_VAR_OFFSET && (x) < VM_VAR_OFFSET + VM_VARS)
#define         VM_IS_TEMP(x)   ((x) >= VM_TEMP_OFFSET && (x) < VM_TEMP_OFFSET + VM_TEMP)

struct        ChoicePts
{
  std::vector<uint8_t>  choices;
  uint8_t               pts;
};

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

  enum          ScriptType
    {
      PERSON,
      SIGN,
      SCRIPT,
      STD,
      NONE
    };

  struct        Identifier
  {
    uint8_t     bank;
    uint8_t     map;
    uint8_t     id;
    ScriptType  type;

    Identifier(uint8_t bank = 0, uint8_t map = 0, uint8_t id = 0, ScriptType type = NONE)
      : bank(bank), map(map), id(id), type(type)
    {}

    bool        operator<(const Identifier &o) const
    {
      if (bank != o.bank)
        return (bank < o.bank);
      if (map != o.map)
        return (map < o.map);
      if (id != o.id)
        return (id < o.id);
      return (type < o.type);
    }
  };

public:
  Script(uint8_t bank = 0, uint8_t map = 0, uint8_t id = 0, ScriptType type = NONE);
  ~Script();

public:
  Script	&load(uint32_t ptr);
  Script	&loadStd(uint8_t n);
  void		print();
  static Script *getStd(uint8_t n) { return (_getScript(0, 0, n, STD)); }
  static Script *getPerson(uint8_t id, uint8_t bank = 0, uint8_t map = 0) { return (_getScript(bank, map, id, PERSON)); }
  static Script *getSign(uint8_t id, uint8_t bank = 0, uint8_t map = 0) { return (_getScript(bank, map, id, SIGN)); }
  static Script *getScript(uint8_t id, uint8_t bank = 0, uint8_t map = 0) { return (_getScript(bank, map, id, SCRIPT)); }
  static Script *getGenericScript(uint8_t id, uint8_t b, uint8_t m, ScriptType t) { return (_getScript(b, m, id, t)); }
  static std::vector<Script *>  &getHooked(uint16_t var) { return (_hookList[var]); }

public:
  uint8_t       getBank() const { return (_id.bank); }
  uint8_t       getMap() const { return (_id.map); }
  uint8_t       getId() const { return (_id.id); }
  uint8_t       getType() const { return (_id.type); }

public:
  std::vector<ChoicePts>        &getChoices() { return (_choices); }
  std::map<int, Instruction *>  &getInstructions() { return (_instructions); }
  uint32_t                      getStartOffset() { return (_offset); }

private:
  void          _subPrint(uint32_t ptr);
  void		_reset();
  bool		_setupNextAddr();
  void          _getInstruction(Command &cmd);
  void          _addHook(Instruction *instr);
  static Script *_getScript(uint8_t bank, uint8_t map, uint8_t id, ScriptType type);
private:
  uint32_t	_readByte();
  uint32_t	_readWord();
  uint32_t	_readDword();

private:
  void          _loadpointer(Instruction *instr);
  void          _bufferstring(Instruction *instr);
  void          _preparemsg(Instruction *instr);
  void          _checkattack(Instruction *instr);
  void          _if(Instruction *instr);
  void          _branch(Instruction *instr);

private:
  Data			&_data;
  Identifier            _id;
  uint32_t		_offset;
  uint32_t		_start;
  uint8_t		*_ptr;
  uint32_t		_pc;
  uint32_t		_oldpc;
  std::vector<Range>	_ranges;
  std::queue<uint32_t>	_addrs;
  std::map<int, Instruction *>  _instructions;
  std::vector<ChoicePts>        _choices;

  static Command	_cmds[0xD6];
  static std::map<Identifier, Script *>         _cache;
  static std::map<std::string, ParamReader>     _readers;
  static std::map<uint8_t, uint8_t>             _cmdHooks;
  static std::map<uint16_t, std::vector<Script *> >     _hookList;
};

#endif
