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

uint32_t	Script::_readPointer()
{
  return (_readDword());
}

uint32_t	Script::_readVariable()
{
  return (_readWord());
}

uint32_t	Script::_readFlag()
{
  return (_readWord());
}

uint32_t	Script::_readBank()
{
  return (_readByte());
}

uint32_t	Script::_readBuffer()
{
  return (_readByte());
}

uint32_t	Script::_readHiddenVar()
{
  return (_readByte());
}

uint32_t	Script::_readPtrOrBank()
{
  return (_readDword());
}

uint32_t	Script::_readPtrOrBank0()
{
  return (_readDword());
}

uint32_t	Script::_readFlagOrVar()
{
  return (_readWord());
}

uint32_t	Script::_readWordOrVar()
{
  return (_readWord());
}

uint32_t	Script::_readByteOrVar()
{
  return (_readWord());
}
