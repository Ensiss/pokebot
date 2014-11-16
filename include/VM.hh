#ifndef         __VM_HH__
#define         __VM_HH__

#include        <list>
#include        <stack>
#include        <cstring>
#include        <functional>
#include        "Script.hh"

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

#define         VM_BOUNDS_ERR(x)        ("Warning: " x " 0x%x out of bounds\n")

class           VM
{
public:
  struct        ChoicePts
  {
    std::vector<uint8_t> choices;
    uint8_t     pts;
  };

private:
  class         Context
  {
  public:
    Context();
    Context(const Context &ctx);
    Context     &operator=(const Context &ctx);

  public:
    void        update();
    bool        getFlag(uint16_t flag) const;
    uint16_t    getVar(uint16_t var) const;
    uint32_t    getBank(uint8_t bank) const;
    void        setFlag(uint16_t flag, bool val);
    void        setVar(uint16_t var, uint16_t val);
    void        setBank(uint8_t bank, uint32_t val);
    void        compare(uint32_t a, uint32_t b) { cmp1 = a; cmp2 = b; }
    void        pushStack(uint32_t addr) { stack.push(addr); }
    uint32_t    popStack() { uint32_t addr = stack.top(); stack.pop(); return (addr); }
    void        clearStack() { while (stack.size()) stack.pop(); }

  private:
    uint8_t     _flags[VM_FLAGS >> 3];
    uint16_t    _vars[VM_VARS];
    uint16_t    _temp[VM_TEMP];
    uint32_t    _banks[VM_BANKS];

  public:
    std::stack<uint32_t> stack;
    uint32_t    pc;
    uint32_t    cmp1;
    uint32_t    cmp2;
    ChoicePts   cpts;
  };

public:
  typedef       void (VM::*Executer)(Script::Instruction *);

public:
  VM();
  ~VM();

public:
  void          exec(Script &script);
  std::vector<ChoicePts>        *execCountNewVisits(Script &script);

public:
  bool          getFlag(uint16_t flag) const { return (_ctx.getFlag(flag)); }
  uint16_t      getVar(uint16_t var) const { return (_ctx.getVar(var)); }
  uint32_t      getBank(uint8_t bank) const { return (_ctx.getBank(bank)); }
  void          setFlag(uint16_t flag, bool val) { _ctx.setFlag(flag, val); }
  void          setVar(uint16_t var, uint16_t val) { _ctx.setVar(var, val); }
  void          setBank(uint8_t bank, uint32_t val) { _ctx.setBank(bank, val); }

private:
  void          _compare(uint32_t a, uint32_t b) { _ctx.compare(a, b); }
  void          _compare8(uint8_t a, uint8_t b) { _ctx.compare(a, b); }
  void          _saveContext() { _states.push(new Context(_ctx)); }
  bool          _restoreContext();

private:
  void          _return(Script::Instruction *instr);
  void          _call(Script::Instruction *instr);
  void          _goto(Script::Instruction *instr);
  void          _if1(Script::Instruction *instr);
  void          _if2(Script::Instruction *instr);

  void          _loadpointer(Script::Instruction *instr)
  { setBank(instr->args[0], instr->args[1]); }
  void          _setbyte2(Script::Instruction *instr)
  { setBank(instr->args[0], instr->args[1]); }
  void          _loadbytefrompointer(Script::Instruction *instr)
  { setBank(instr->args[0], *(uint8_t *) gbaMem(instr->args[1])); }
  void          _copyscriptbanks(Script::Instruction *instr)
  { setBank(instr->args[0], getBank(instr->args[1])); }
  void          _setvar(Script::Instruction *instr)
  { setVar(instr->args[0], instr->args[1]); }
  void          _addvar(Script::Instruction *instr)
  { setVar(instr->args[0], getVar(instr->args[0]) + instr->args[1]); }
  void          _subvar(Script::Instruction *instr)
  { setVar(instr->args[0], getVar(instr->args[0]) - instr->args[1]); }
  void          _copyvar(Script::Instruction *instr)
  { setVar(instr->args[0], getVar(instr->args[1])); }
  void          _copyvarifnotzero(Script::Instruction *instr)
  { setVar(instr->args[0], VM_IS_VAR(instr->args[1]) ? getVar(instr->args[1]) : instr->args[1]); }
  void          _comparebanks(Script::Instruction *instr)
  { _compare8(getBank(instr->args[0]), getBank(instr->args[1])); }
  void          _comparebanktobyte(Script::Instruction *instr)
  { _compare8(getBank(instr->args[0]), instr->args[1]); }
  void          _comparebanktofarbyte(Script::Instruction *instr)
  { _compare8(getBank(instr->args[0]), (*(uint8_t *) gbaMem(instr->args[1]))); }
  void          _comparefarbytetobank(Script::Instruction *instr)
  { _compare8((*(uint8_t *) gbaMem(instr->args[0])), getBank(instr->args[1])); }
  void          _comparefarbytetobyte(Script::Instruction *instr)
  { _compare8((*(uint8_t *) gbaMem(instr->args[0])), instr->args[1]); }
  void          _comparefarbytes(Script::Instruction *instr)
  { _compare8((*(uint8_t *) gbaMem(instr->args[0])), (*(uint8_t *) gbaMem(instr->args[1]))); }
  void          _compare(Script::Instruction *instr)
  { _compare(getVar(instr->args[0]), instr->args[1]); }
  void          _comparevars(Script::Instruction *instr)
  { _compare(getVar(instr->args[0]), getVar(instr->args[1])); }
  void          _setflag(Script::Instruction *instr)
  { setFlag(instr->args[0], 1); }
  void          _clearflag(Script::Instruction *instr)
  { setFlag(instr->args[0], 0); }
  void          _checkflag(Script::Instruction *instr)
  { _compare(getFlag(instr->args[0]), 1); }
  void          _resetvars(Script::Instruction *instr)
  { for (int i = 0x8000; i <= 0x8002; i++) setVar(i, 0); }

private:
  Context               _ctx;
  std::stack<Context *> _states;

  static Executer       _executers[0xD6];
  static std::function<bool(uint32_t, uint32_t)>  _cmpOp[6];
};

#endif
