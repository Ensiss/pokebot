#include        <iostream>
#include        <string>
#include        <LuaBridge.h>
#include        "Data.hh"
#include        "Bot.hh"
#include        "BotUtils.hh"
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

private:
  void          _error(int ret);

public:
  void          init();
  lua_State     *getState() const { return (_state); }
  void          doFile(const std::string &s);
  void          doString(const std::string &s);
  void          doFunc(const std::string &s);

private:
  lua_State     *_state;
};
