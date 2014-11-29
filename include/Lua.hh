#include        <iostream>
#include        <string>
#include        <LuaBridge.h>
#include        "Data.hh"
#include        "Bot.hh"
#include        "BotUtils.hh"
#include        "Action/AAction.hh"
#include        "Action/LuaClass.hh"
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
  void          _initButtons();
  void          _initStates();
  void          _initWildTypes();
  void          _pushvar(const char *name, uint8_t val);

public:
  void          init();
  lua_State     *getState() const { return (_state); }
  void          doFile(const std::string &s);
  void          doString(const std::string &s);
  void          doFunc(const std::string &s);
  void          doREPL();

private:
  lua_State     *_state;
};
