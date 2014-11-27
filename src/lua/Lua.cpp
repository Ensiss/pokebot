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

    .beginClass<World>("World")
    .addFunction("getMap", &World::getMap)
    .endClass()
    .addVariable("world", &Action::data->world())

    .beginClass<World::Map::Node>("Node")
    .addFunction("getStatus", &World::Map::Node::getStatus)
    .addFunction("getBehavior", &World::Map::Node::getBehavior)
    .addFunction("getBackground", &World::Map::Node::getBackground)
    .addFunction("getX", &World::Map::Node::getX)
    .addFunction("getY", &World::Map::Node::getY)
    .endClass()

    .beginClass<World::Map>("Map")
    .addFunction("getWidth", &World::Map::getWidth)
    .addFunction("getHeight", &World::Map::getHeight)
    .addFunction("getNbPersons", &World::Map::getNbPersons)
    .addFunction("getNbWarps", &World::Map::getNbWarps)
    .addFunction("getNbScripts", &World::Map::getNbScripts)
    .addFunction("getNbSigns", &World::Map::getNbSigns)
    .addFunction("getNbConnections", &World::Map::getNbConnections)
    .addFunction("getScripPtr", &World::Map::getScripPtr)
    .addFunction("getLabelId", &World::Map::getLabelId)
    .addFunction("getName", &World::Map::getName)
    .addFunction("getNode", &World::Map::getNode)
    .endClass()
    .endNamespace()

    .beginNamespace("bot")
    .addFunction("clear", &BotUtils::clear)
    .addFunction("moveTo", &BotUtils::moveTo)
    .addFunction("talkTo", &BotUtils::talkTo)
    .addFunction("useWarp", &BotUtils::useWarp)
    .addFunction("pressButton", &BotUtils::pressButton)
    .addFunction("wait", &BotUtils::wait)
    .endNamespace()
    ;

  _initButtons();
  doFile("lua/main.lua");
}

void            Lua::_pushvar(const char *name, uint8_t val)
{
  lua_pushstring(_state, name);
  lua_pushnumber(_state, val);
  lua_settable(_state, -3);
}

void            Lua::_initButtons()
{
  // Get global namespace
  lua_getglobal(_state, "_G");
  // Create "btn" namespace
  lua_pushstring(_state, "btn");
  lua_newtable(_state);

  // Push variables
  _pushvar("left", KEY_LEFT);
  _pushvar("right", KEY_RIGHT);
  _pushvar("up", KEY_UP);
  _pushvar("down", KEY_DOWN);
  _pushvar("a", KEY_BUTTON_A);
  _pushvar("b", KEY_BUTTON_B);
  _pushvar("start", KEY_BUTTON_START);
  _pushvar("select", KEY_BUTTON_SELECT);

  lua_settable(_state, -3);
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
