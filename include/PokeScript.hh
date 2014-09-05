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
#include	"Data.hh"

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
  PokeScript(Data &data);
  ~PokeScript();

public:
  void		print(uint32_t ptr);
  void		printStd(uint8_t n);

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
  void		_jumpram() {
    _print("jumpram"); }
  void		_loadpointer();
  void		_setvar() { uint16_t v = _readVariable();
    _print("setvar %#x %#x", v, _readWord()); }
  void		_addvar() { uint16_t v = _readVariable();
    _print("addvar %#x %#x", v, _readWord()); }
  void		_subvar() { uint16_t v = _readVariable();
    _print("subvar %#x %#x", v, _readWord()); }
  void		_copyvar() { uint16_t v = _readVariable();
    _print("copyvar %#x %#x", v, _readVariable()); }
  void		_copyvarifnotzero() { uint16_t v = _readVariable();
    _print("copyvarifnotzero %#x %#x", v, _readWordOrVar()); }
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
  void		_checksound() {
    _print("checksound"); }
  void		_fanfare() {
    _print("fanfare %#04x", _readWordOrVar()); }
  void		_waitfanfare() {
    _print("waitfanfare"); }
  void		_playsound() { uint16_t s = _readWord();
    _print("playsound %#x %x", s, _readByte()); }
  void		_playsound2() { uint16_t s = _readWord();
    _print("playsound2 %#x", s); }
  void		_warp();
  void		_additem() { uint16_t w = _readWordOrVar();
    _print("additem #%x x%d", w, _readByteOrVar()); }
  void		_removeitem() { uint16_t w = _readWordOrVar();
    _print("removeitem #%x x%d", w, _readByteOrVar()); }
  void		_checkitemroom() { uint16_t w = _readWordOrVar();
    _print("checkitemroom #%x x%d", w, _readByteOrVar()); }
  void		_checkitem() { uint16_t w = _readWordOrVar();
    _print("checkitem #%x x%d", w, _readByteOrVar()); }
  void		_checkitemtype() { uint16_t w = _readWordOrVar();
    _print("checkitemtype #%x", w); }
  void		_applymovement() { uint8_t id = _readByteOrVar();
    _print("applymovement %d %#08x", id, _readPointer()); }
  void		_waitmovement() {
    _print("waitmovement %d", _readByteOrVar()); }
  void		_hidesprite() {
    _print("hidesprite %d", _readByteOrVar()); }
  void		_showsprite() {
    _print("showsprite %d", _readByteOrVar()); }
  void		_faceplayer() {
    _print("faceplayer"); }
  void		_movesprite2() { uint16_t w = _readWord(); uint16_t w2 = _readWord();
    _print("movesprite2 %d %d %d", w, w2, _readWord()); }
  void		_moveoffscreen() {
    _print("moveoffscreen %d", _readWord()); }
  void		_spritebehave() { uint8_t w = _readWord();
    _print("spritebehave %d %d", w, _readByte()); }
  void		_waitmsg() {
    _print("waitmsg"); }
  void		_preparemsg();
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
  void		_checkattack() {
    _print("checkattack %s", _data.move(_readWord()).getName()); }
  void		_bufferpokemon() { uint8_t b = _readBuffer();
    _print("bufferpokemon %d %x", b, _readWordOrVar()); }
  void		_bufferfirstpokemon() { uint8_t b = _readBuffer();
    _print("bufferfirstpokemon %d", b); }
  void		_bufferpartypokemon() { uint8_t b = _readBuffer();
    _print("bufferpartypokemon %d %#x", b, _readWordOrVar()); }
  void		_bufferitem() { uint8_t b = _readBuffer();
    _print("bufferitem %d %x", b, _readWordOrVar()); }
  void		_bufferdecoration() { _readByte(), _readWordOrVar();
    _print("bufferdecoration (nop)"); }
  void		_bufferattack() { uint8_t b = _readBuffer();
    _print("bufferattack %d %d", b, _readWordOrVar()); }
  void		_buffernumber() { uint8_t b = _readBuffer();
    _print("buffernumber %d %d", b, _readWordOrVar()); }
  void		_bufferstd() { uint8_t b = _readBuffer();
    _print("bufferpokemon %d %d", b, _readWordOrVar()); }
  void		_bufferstring();
  void		_doanimation() {
    _print("doanimation %#06x", _readWord()); }
  void		_setanimation() { uint8_t b = _readByte();
    _print("setanimation 0x%x %#06x", b, _readWordOrVar()); }
  void		_checkanimation() {
    _print("checkanimation %#06x", _readWord()); }
  void		_setdooropened() { uint16_t x = _readWord();
    _print("setdooropened %d,%d", x, _readWord()); }
  void		_setdoorclosed() { uint16_t x = _readWord();
    _print("setdoorclosed %d,%d", x, _readWord()); }
  void		_doorchange() {
    _print("doorchange"); }
  void		_setdooropened2() { uint16_t x = _readWord();
    _print("setdooropened2 %d,%d", x, _readWord()); }
  void		_setdoorclosed2() { uint16_t x = _readWord();
    _print("setdoorclosed2 %d,%d", x, _readWord()); }
  void		_textcolor() {
    _print("textcolor %d", _readByte()); }
  void		_signmsg() {
    _print("signmsg"); }
  void		_normalmsg() {
    _print("normalmsg"); }

private:
  typedef void		(PokeScript::*Instruction)();
  Data			&_data;
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
