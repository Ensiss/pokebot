#ifndef		__POKESCRIPT_HH__
#define		__POKESCRIPT_HH__

#include	<string>
#include	<cstdio>
#include	<stdarg.h>
#include	<stdint.h>
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"

class		PokeScript
{
public:
  PokeScript(uint32_t ptr);
  ~PokeScript();

public:
  void		print();
  void		reset();

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
  void		_nop() {
    _print("nop"); }
  void		_end() {
    _print("end"); }
  void		_return() {
    _print("return"); }
  void		_call() {
    _print("call %#08x", _readPointer()); }
  void		_goto() {
    _print("goto %#08x", _readPointer()); }
  void		_if1() { uint8_t b = _readByte();
    _print("if %#02x goto %#08x", b, _readPointer()); }
  void		_if2() { uint8_t b = _readByte();
    _print("if %#02x call %#08x", b, _readPointer()); }
  void		_gotostd() {
    _print("gotostd %#02x", _readByte()); }
  void		_callstd() {
    _print("callstd %#02x", _readByte()); }
  void		_gotostdif() { uint8_t b = _readByte();
    _print("if %#02x gotostd %#02x", b, _readByte()); }
  void		_callstdif() { uint8_t b = _readByte();
    _print("if %#02x callstd %#02x", b, _readByte()); }
  void		_compare() { uint16_t v = _readVariable();
    _print("compare %#04x %#04x", v, _readWord()); }
  void		_lock() {
    _print("lock"); }

private:
  typedef void		(PokeScript::*Instruction)();
  uint32_t		_offset;
  uint8_t		*_ptr;
  uint32_t		_pc;
  uint32_t		_oldpc;
  static Instruction	_inst[0xD6];
};

#endif
