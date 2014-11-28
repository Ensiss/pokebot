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

    .beginClass<World::Map>("Map")
    .addFunction("getWidth", &World::Map::getWidth)
    .addFunction("getHeight", &World::Map::getHeight)
    .addFunction("getNbPersons", &World::Map::getNbPersons)
    .addFunction("getNbWarps", &World::Map::getNbWarps)
    .addFunction("getNbScripts", &World::Map::getNbScripts)
    .addFunction("getNbSigns", &World::Map::getNbSigns)
    .addFunction("getNbConnections", &World::Map::getNbConnections)
    .addFunction("getPerson", &World::Map::getPerson)
    .addFunction("getWarp", &World::Map::getWarp)
    .addFunction("getScript", &World::Map::getScript)
    .addFunction("getSign", &World::Map::getSign)
    .addFunction("getConnection", &World::Map::getConnection)
    .addFunction("getScripPtr", &World::Map::getScripPtr)
    .addFunction("getLabelId", &World::Map::getLabelId)
    .addFunction("getName", &World::Map::getName)
    .addFunction("getNode", &World::Map::getNode)
    .endClass()
    .endNamespace()

    .beginClass<World::Map::Node>("Node")
    .addFunction("getStatus", &World::Map::Node::getStatus)
    .addFunction("getBehavior", &World::Map::Node::getBehavior)
    .addFunction("getBackground", &World::Map::Node::getBackground)
    .addFunction("getX", &World::Map::Node::getX)
    .addFunction("getY", &World::Map::Node::getY)
    .endClass()

    .beginClass<World::SignEvt>("Sign")
    .addFunction("getX", &World::SignEvt::getX)
    .addFunction("getY", &World::SignEvt::getY)
    .addFunction("getLevel", &World::SignEvt::getLevel)
    .addFunction("getType", &World::SignEvt::getType)
    .addFunction("getScript", &World::SignEvt::getScript)
    .endClass()

    .beginClass<World::WarpEvt>("Warp")
    .addFunction("getX", &World::WarpEvt::getX)
    .addFunction("getY", &World::WarpEvt::getY)
    .addFunction("getLevel", &World::WarpEvt::getLevel)
    .addFunction("getWarp", &World::WarpEvt::getWarp)
    .addFunction("getMap", &World::WarpEvt::getMap)
    .addFunction("getBank", &World::WarpEvt::getBank)
    .endClass()

    .beginClass<World::PersonEvt>("Person")
    .addFunction("getEventNb", &World::PersonEvt::getEventNb)
    .addFunction("getX", &World::PersonEvt::getX)
    .addFunction("getY", &World::PersonEvt::getY)
    .addFunction("getLevel", &World::PersonEvt::getLevel)
    .addFunction("getMovementType", &World::PersonEvt::getMovementType)
    .addFunction("getMovement", &World::PersonEvt::getMovement)
    .addFunction("isTrainer", &World::PersonEvt::isTrainer)
    .addFunction("getView", &World::PersonEvt::getView)
    .addFunction("getScript", &World::PersonEvt::getScript)
    .addFunction("getId", &World::PersonEvt::getId)
    .endClass()

    .beginClass<World::ScriptEvt>("Script")
    .addFunction("getX", &World::ScriptEvt::getX)
    .addFunction("getY", &World::ScriptEvt::getY)
    .addFunction("getLevel", &World::ScriptEvt::getLevel)
    .addFunction("getVarNb", &World::ScriptEvt::getVarNb)
    .addFunction("getVarValue", &World::ScriptEvt::getVarValue)
    .addFunction("getScript", &World::ScriptEvt::getScript)
    .endClass()

    .beginClass<World::Connection>("Connection")
    .addFunction("getType", &World::Connection::getType)
    .addFunction("getOffset", &World::Connection::getOffset)
    .addFunction("getBank", &World::Connection::getBank)
    .addFunction("getMap", &World::Connection::getMap)
    .endClass()

    .beginNamespace("bot")
    .addFunction("queue", &BotUtils::queue)
    .addFunction("clear", &BotUtils::clear)
    .endNamespace()

    .beginNamespace("mem")
    .addFunction("get8", &gbaMem<uint8_t>)
    .addFunction("get16", &gbaMem<uint16_t>)
    .addFunction("get32", &gbaMem<uint32_t>)
    .endNamespace()

    .beginNamespace("new")
    .addFunction("moveTo", &Action::MoveTo::create)
    .addFunction("talkTo", &Action::TalkTo::create)
    .addFunction("useWarp", &Action::UseWarp::create)
    .addFunction("pressButton", &Action::PressButton::create)
    .addFunction("wait", &Action::Wait::create)
    .addFunction("lua", &Action::LuaClass::create)
    .endNamespace()

    .beginNamespace("action")
    .beginClass<AAction>("AAction")
    .addFunction("queue", &AAction::queue)
    .endClass()
    .deriveClass<Action::MoveTo, AAction>("MoveTo").endClass()
    .deriveClass<Action::TalkTo, AAction>("TalkTo").endClass()
    .deriveClass<Action::UseWarp, AAction>("UseWarp").endClass()
    .deriveClass<Action::PressButton, AAction>("PressButton").endClass()
    .deriveClass<Action::Wait, AAction>("Wait").endClass()
    .deriveClass<Action::LuaClass, AAction>("LuaClass").endClass()
    .endNamespace()
    ;

  _initButtons();
  _initStates();
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

void            Lua::_initStates()
{
  // Get global namespace
  lua_getglobal(_state, "_G");
  // Create "state" namespace
  lua_pushstring(_state, "state");
  lua_newtable(_state);

  // Push variables
  _pushvar("not_started", Action::NOT_STARTED);
  _pushvar("running", Action::RUNNING);
  _pushvar("finished", Action::FINISHED);
  _pushvar("error", Action::ERROR);

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
