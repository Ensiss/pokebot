#include        "Script.hh"

std::map<std::string, Script::ParamReader> Script::_readers = {
  {"byte", &Script::_readByte},
  {"word", &Script::_readWord},
  {"dword", &Script::_readDword},
  {"ptr", &Script::_readDword},
  {"var", &Script::_readWord},
  {"flag", &Script::_readWord},
  {"bank", &Script::_readByte},
  {"buffer", &Script::_readByte},
  {"hidden", &Script::_readByte},
  {"ptr/bank", &Script::_readDword},
  {"ptr/bank0", &Script::_readDword},
  {"flag/var", &Script::_readWord},
  {"word/var", &Script::_readWord},
  {"byte/var", &Script::_readWord}
};

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
