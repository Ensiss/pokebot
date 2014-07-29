#include	"PokeScript.hh"

uint8_t		PokeScript::_readByte()
{
  uint8_t	ret = *((uint8_t *) (_ptr + _pc));
  _pc += 1;
  return (ret);
}

uint16_t	PokeScript::_readWord()
{
  uint16_t	ret = *((uint16_t *) (_ptr + _pc));
  _pc += 2;
  return (ret);
}

uint32_t	PokeScript::_readDword()
{
  uint32_t	ret = *((uint32_t *) (_ptr + _pc));
  _pc += 4;
  return (ret);
}

uint32_t	PokeScript::_readPointer()
{
  return (_readDword());
}

uint16_t	PokeScript::_readVariable()
{
  return (_readWord());
}

uint16_t	PokeScript::_readFlag()
{
  return (_readWord());
}

uint8_t		PokeScript::_readBank()
{
  return (_readByte());
}

uint8_t		PokeScript::_readBuffer()
{
  return (_readByte());
}

uint8_t		PokeScript::_readHiddenVar()
{
  return (_readByte());
}

uint32_t	PokeScript::_readPtrOrBank()
{
  return (_readDword());
}

uint32_t	PokeScript::_readPtrOrBank0()
{
  return (_readDword());
}

uint16_t	PokeScript::_readFlagOrVar()
{
  return (_readWord());
}

uint16_t	PokeScript::_readWordOrVar()
{
  return (_readWord());
}

uint8_t		PokeScript::_readByteOrVar()
{
  return (_readWord());
}
