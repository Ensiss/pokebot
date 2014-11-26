#include        "Lua.hh"

Lua             L;
extern Bot      bot;

Lua::Lua()
{
  _state = luaL_newstate();
  luaL_openlibs(_state);
}

Lua::~Lua()
{
}

void            Lua::init()
{
  getGlobalNamespace(_state)
    .beginNamespace("data")
    .beginClass<Player>("Player")
    .addFunction("getX", &Player::getX)
    .addFunction("getY", &Player::getY)
    .addFunction("getBank", &Player::getBank)
    .addFunction("getMap", &Player::getMap)
    .endClass()
    .addVariable("player", &Action::data->player())
    .endNamespace()

    .beginNamespace("bot")
    .addFunction("moveTo", &BotUtils::moveTo)
    .addFunction("talkTo", &BotUtils::talkTo)
    .addFunction("useWarp", &BotUtils::useWarp)
    .addFunction("pressButton", &BotUtils::pressButton)
    .addFunction("wait", &BotUtils::wait)
    .endNamespace()
    ;
  doFile("lua/main.lua");
}

void            Lua::_error(int ret)
{
  if (ret)
    {
      std::cerr << "ERR: " << lua_tostring(_state, ret) << std::endl;
      lua_pop(_state, 1);
    }
}

void            Lua::doFile(const std::string &s)
{
  _error(luaL_dofile(_state, s.c_str()));
}

void            Lua::doString(const std::string &s)
{
  _error(luaL_dostring(_state, s.c_str()));
}

void            Lua::doFunc(const std::string &s)
{
  LuaRef        func = getGlobal(_state, s.c_str());

  if (func.isNil())
    return;
  func();
}

void            Lua::doREPL()
{
  std::string   in;

  std::cout << "Lua REPL for Pokebot. Hit ^D to exit" << std::endl;
  while (1)
    {
      std::cout << "> " << std::flush;
      std::getline(std::cin, in);
      if (!in.size())
        {
          std::cin.clear();
          break;
        }
      doString(in);
    }
}
