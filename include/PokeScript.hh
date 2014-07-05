#ifndef		__POKESCRIPT_HH__
#define		__POKESCRIPT_HH__

#include	<string>
#include	<cstdio>
#include	<stdarg.h>
#include	<stdint.h>
#include	<vector>
#include	<queue>

#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"

class		PokeScript
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
  PokeScript(uint32_t ptr);
  ~PokeScript();

public:
  void		print();

private:
  void		_reset();
  bool		_setupNextAddr();

private:
  uint8_t	_readByte();
  uint16_t	_readWord();
  uint32_t	_readDword();
  uint32_t	_readPointer();
  uint16_t	_readVariable();
  uint16_t	_readFlag();
  uint8_t	_readBank();
  uint8_t	_readBuffer();
  uint8_t	_readHiddenVar();
  uint32_t	_readPtrOrBank();
  uint32_t	_readPtrOrBank0();
  uint16_t	_readFlagOrVar();
  uint16_t	_readWordOrVar();
  uint8_t	_readByteOrVar();

private:
  void		_initInstructions();
  void		_print(const char *s, ...);
  const char	*_cmpOp(uint8_t b);
  void		_nop() {
    _print("nop"); }
  void		_end() {
    _print("end"); }
  void		_return() {
    _print("return"); }
  void		_call();
  void		_goto();
  void		_if1();
  void		_if2();
  void		_gotostd() {
    _print("gotostd %#02x", _readByte()); }
  void		_callstd() {
    _print("callstd %#02x", _readByte()); }
  void		_gotostdif() { const char *op = _cmpOp(_readByte());
    _print("if %s gotostd %#02x", op, _readByte()); }
  void		_callstdif() { const char *op = _cmpOp(_readByte());
    _print("if %s callstd %#02x", op, _readByte()); }
  void		_loadpointer() { uint8_t b = _readBank();
    _print("loadpointer %d %#08x", b, _readDword()); }
  void		_setvar() { uint16_t v = _readVariable();
    _print("setvar %#x %#x", v, _readWord()); }
  void		_addvar() { uint16_t v = _readVariable();
    _print("addvar %#x %#x", v, _readWord()); }
  void		_subvar() { uint16_t v = _readVariable();
    _print("subvar %#x %#x", v, _readWord()); }
  void		_compare() { uint16_t v = _readVariable();
    _print("compare %#04x %#04x", v, _readWord()); }
  void		_special() {
    _print("special %#04x", _readWord()); }
  void		_special2() { uint16_t v = _readVariable();
    _print("special2 %#04x %#04x", v, _readWord()); }
  void		_waitstate() {
    _print("waitstate"); }
  void		_pause() {
    _print("pause %d", _readByte()); }
  void		_setflag() {
    _print("setflag %#x", _readFlagOrVar()); }
  void		_clearflag() {
    _print("clearflag %#x", _readFlagOrVar()); }
  void		_checkflag() {
    _print("checkflag %#x", _readFlagOrVar()); }
  void		_sound() {
    _print("sound %#04x", _readWord()); }
  void		_applymovement() { uint8_t id = _readByteOrVar();
    _print("applymovement %d %#08x", id, _readPointer()); }
  void		_waitmovement() {
    _print("waitmovement %d", _readByteOrVar()); }
  void		_moveoffscreen() {
    _print("moveoffscreen %d", _readWord()); }
  void		_closeonkeypress() {
    _print("closeonkeypress"); }
  void		_lockall() {
    _print("lockall"); }
  void		_lock() {
    _print("lock"); }
  void		_releaseall() {
    _print("releaseall"); }
  void		_release() {
    _print("release"); }
  void		_waitkeypress() {
    _print("waitkeypress"); }
  void		_textcolor() {
    _print("textcolor %d", _readByte()); }
  void		_signmsg() {
    _print("signmsg"); }
  void		_normalmsg() {
    _print("normalmsg"); }

private:
  typedef void		(PokeScript::*Instruction)();
  uint32_t		_offset;
  uint32_t		_start;
  uint8_t		*_ptr;
  uint32_t		_pc;
  uint32_t		_oldpc;
  std::vector<Range>	_ranges;
  std::queue<uint32_t>	_addrs;
  static Instruction	_inst[0xD6];
};

#endif
