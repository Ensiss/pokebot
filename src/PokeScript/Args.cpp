#include        "Script.hh"

uint32_t	Script::_readByte()
{
  uint8_t	ret = *((uint8_t *) (_ptr + _pc));
  _pc += 1;
  return (ret);
}

uint32_t	Script::_readWord()
{
  uint16_t	ret = *((uint16_t *) (_ptr + _pc));
  _pc += 2;
  return (ret);
}

uint32_t	Script::_readDword()
{
  uint32_t	ret = *((uint32_t *) (_ptr + _pc));
  _pc += 4;
  return (ret);
}
