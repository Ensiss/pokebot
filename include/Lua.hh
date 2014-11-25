#include        <string>
#include        <LuaBridge.h>
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
  lua_State     *getState() const { return (_state); }
  void          doFile(const std::string &s);
  void          doString(const std::string &s);

private:
  lua_State     *_state;
};
