#include        <iostream>
#include        <string>
#include        <LuaBridge.h>
#include        "Data.hh"
#include        "Action/AAction.hh"
extern "C" {
#include        "lua.h"
#include        "lauxlib.h"
#include        "lualib.h"
}

using namespace luabridge;

class           Lua
{
public:
  Lua();
  ~Lua();

public:
  void          init();
  lua_State     *getState() const { return (_state); }
  void          doFile(const std::string &s);
  void          doString(const std::string &s);

private:
  lua_State     *_state;
};
