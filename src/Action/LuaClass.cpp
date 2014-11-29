#include        "Action/LuaClass.hh"

Action::LuaClass::LuaClass(LuaRef ref)
  : _ref(ref), _initFunc(_ref["init"]), _updateFunc(_ref["update"])
{
  if (_initFunc.isNil())
    std::cerr << "Warning: constructing LuaClass object with no init function" << std::endl;
  if (_updateFunc.isNil())
    std::cerr << "Warning: constructing LuaClass object with no init function" << std::endl;
}

Action::LuaClass::~LuaClass()
{
}

void            Action::LuaClass::_init()
{
  if (!_initFunc.isNil())
    {
      LuaRef    ret = _initFunc(_ref, this);
      if (ret.isNumber())
        _state = (Action::State) ret.cast<uint8_t>();
    }
}

void            Action::LuaClass::_update()
{
  if (!_updateFunc.isNil())
    {
      LuaRef    ret = _updateFunc(_ref, this);
      if (ret.isNumber())
        _state = (Action::State) ret.cast<uint8_t>();
    }
}
