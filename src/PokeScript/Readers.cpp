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
