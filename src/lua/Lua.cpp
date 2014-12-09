#include        "Lua.hh"

Lua             L;

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
    .addFunction("getOverWorld", &Data::getOverWorldWrapper)
    .addFunction("getSpecies", &Data::getSpeciesWrapper)
    .addFunction("getMove", &Data::getMoveWrapper)
    .addFunction("getType", &Data::getTypeWrapper)
    .addFunction("getAbilityName", &Data::getAbilityNameWrapper)
    .addFunction("getPlayerPokemon", &Data::getPlayerPokemonWrapper)
    .addFunction("getEnemyPokemon", &Data::getEnemyPokemonWrapper)
    .addFunction("getBattler", &Data::getBattlerWrapper)
    .addFunction("getCurrentMap", &Data::getCurrentMap)
    .addFunction("getMap", &Data::getMap)

    .beginClass<Player>("Player")
    .addFunction("getX", &Player::getX)
    .addFunction("getY", &Player::getY)
    .addFunction("getBank", &Player::getBank)
    .addFunction("getMap", &Player::getMap)
    .endClass()
    .addVariable("player", &Data::data->player())

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
    .addFunction("getWildBattle", &World::Map::getWildBattle)
    .endClass()
    .endNamespace()

    .beginClass<IPokeData>("IPokeData")
    .addFunction("getNick", &IPokeData::getNick)
    .addFunction("getOtName", &IPokeData::getOtName)
    .addFunction("getRealAtk", &IPokeData::getRealAtk)
    .addFunction("getRealDef", &IPokeData::getRealDef)
    .addFunction("getRealSpeed", &IPokeData::getRealSpeed)
    .addFunction("getRealSpAtk", &IPokeData::getRealSpAtk)
    .addFunction("getRealSpDef", &IPokeData::getRealSpDef)
    .addFunction("getRealAccuracy", &IPokeData::getRealAccuracy)
    .addFunction("getRealEvasion", &IPokeData::getRealEvasion)
    .addFunction("isSleeping", &IPokeData::isSleeping)
    .addFunction("isPoisoned", &IPokeData::isPoisoned)
    .addFunction("isBurnt", &IPokeData::isBurnt)
    .addFunction("isFreezed", &IPokeData::isFreezed)
    .addFunction("isParalysed", &IPokeData::isParalysed)
    .addFunction("isBadlyPoisoned", &IPokeData::isBadlyPoisoned)
    .addFunction("getMove", &IPokeData::getMove)
    .addFunction("getPP", &IPokeData::getPP)
    .endClass()

    .deriveClass<PokemonData, IPokeData>("PokemonData")
    .addFunction("getStatus", &PokemonData::getStatus)
    .addFunction("hasPokerus", &PokemonData::hasPokerus)
    .addFunction("getLevel", &PokemonData::getLevel)
    .addFunction("getHP", &PokemonData::getHP)
    .addFunction("getMaxHP", &PokemonData::getMaxHP)
    .addFunction("getAtk", &PokemonData::getAtk)
    .addFunction("getDef", &PokemonData::getDef)
    .addFunction("getSpeed", &PokemonData::getSpeed)
    .addFunction("getSpAtk", &PokemonData::getSpAtk)
    .addFunction("getSpDef", &PokemonData::getSpDef)
    .addFunction("getHPBuff", &PokemonData::getHPBuff)
    .addFunction("getAtkBuff", &PokemonData::getAtkBuff)
    .addFunction("getDefBuff", &PokemonData::getDefBuff)
    .addFunction("getSpeedBuff", &PokemonData::getSpeedBuff)
    .addFunction("getSpAtkBuff", &PokemonData::getSpAtkBuff)
    .addFunction("getSpDefBuff", &PokemonData::getSpDefBuff)
    .addFunction("getAccuracyBuff", &PokemonData::getAccuracyBuff)
    .addFunction("getEvasionBuff", &PokemonData::getEvasionBuff)
    .addFunction("getSpecies", &PokemonData::getSpecies)
    .addFunction("getItem", &PokemonData::getItem)
    .addFunction("getXP", &PokemonData::getXP)
    .addFunction("getHPEV", &PokemonData::getHPEV)
    .addFunction("getAtkEV", &PokemonData::getAtkEV)
    .addFunction("getDefEV", &PokemonData::getDefEV)
    .addFunction("getSpeedEV", &PokemonData::getSpeedEV)
    .addFunction("getSpAtkEV", &PokemonData::getSpAtkEV)
    .addFunction("getSpDefEV", &PokemonData::getSpDefEV)
    .endClass()

    .deriveClass<BattleData, IPokeData>("BattleData")
    .addFunction("getStatus", &BattleData::getStatus)
    .addFunction("getLevel", &BattleData::getLevel)
    .addFunction("getSpecies", &BattleData::getSpecies)
    .addFunction("getHP", &BattleData::getHP)
    .addFunction("getMaxHP", &BattleData::getMaxHP)
    .addFunction("getAtk", &BattleData::getAtk)
    .addFunction("getDef", &BattleData::getDef)
    .addFunction("getSpeed", &BattleData::getSpeed)
    .addFunction("getSpAtk", &BattleData::getSpAtk)
    .addFunction("getSpDef", &BattleData::getSpDef)
    .addFunction("getHPBuff", &BattleData::getHPBuff)
    .addFunction("getAtkBuff", &BattleData::getAtkBuff)
    .addFunction("getDefBuff", &BattleData::getDefBuff)
    .addFunction("getSpeedBuff", &BattleData::getSpeedBuff)
    .addFunction("getSpAtkBuff", &BattleData::getSpAtkBuff)
    .addFunction("getSpDefBuff", &BattleData::getSpDefBuff)
    .addFunction("getAccuracyBuff", &BattleData::getAccuracyBuff)
    .addFunction("getEvasionBuff", &BattleData::getEvasionBuff)
    .endClass()

    .beginClass<OverWorld>("OverWorld")
    .addFunction("getDestX", &OverWorld::getDestX)
    .addFunction("getDestY", &OverWorld::getDestY)
    .addFunction("getCurrX", &OverWorld::getCurrX)
    .addFunction("getCurrY", &OverWorld::getCurrY)
    .addFunction("getX", &OverWorld::getX)
    .addFunction("getY", &OverWorld::getY)
    .addFunction("getDir", &OverWorld::getDir)
    .addFunction("getFlags", &OverWorld::getFlags)
    .addFunction("getEventNb", &OverWorld::getEventNb)
    .addFunction("getMap", &OverWorld::getMap)
    .addFunction("getBank", &OverWorld::getBank)
    .addFunction("getSpawnX", &OverWorld::getSpawnX)
    .addFunction("getSpawnY", &OverWorld::getSpawnY)
    .endClass()

    .beginClass<Move>("Move")
    .addFunction("getPower", &Move::getPower)
    .addFunction("getType", &Move::getType)
    .addFunction("getAccuracy", &Move::getAccuracy)
    .addFunction("getPP", &Move::getPP)
    .addFunction("getEffectAccuracy", &Move::getEffectAccuracy)
    .addFunction("getTarget", &Move::getTarget)
    .addFunction("getPriority", &Move::getPriority)
    .addFunction("getFlags", &Move::getFlags)
    .addFunction("getName", &Move::getName)
    .endClass()

    .beginClass<Species>("Species")
    .addFunction("getHP", &Species::getHP)
    .addFunction("getAtk", &Species::getAtk)
    .addFunction("getDef", &Species::getDef)
    .addFunction("getSpeed", &Species::getSpeed)
    .addFunction("getSpAtk", &Species::getSpAtk)
    .addFunction("getSpDef", &Species::getSpDef)
    .addFunction("getType", &Species::getType)
    .addFunction("getAbility", &Species::getAbility)
    .addFunction("getItem", &Species::getItem)
    .addFunction("getName", &Species::getName)
    .endClass()

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
    .addFunction("getVariable", &World::ScriptEvt::getVarNb)
    .addFunction("getValue", &World::ScriptEvt::getVarValue)
    .addFunction("getScript", &World::ScriptEvt::getScript)
    .endClass()

    .beginClass<World::Connection>("Connection")
    .addFunction("getType", &World::Connection::getType)
    .addFunction("getOffset", &World::Connection::getOffset)
    .addFunction("getBank", &World::Connection::getBank)
    .addFunction("getMap", &World::Connection::getMap)
    .endClass()

    .beginClass<World::Map::WildBattle>("WildBattle")
    .addFunction("getRatio", &World::Map::WildBattle::getRatio)
    .addFunction("getNbEntries", &World::Map::WildBattle::getNbEntries)
    .addFunction("getEntry", &World::Map::WildBattle::getEntry)
    .endClass()

    .beginClass<World::WildEntry>("WildEntry")
    .addFunction("getMinLevel", &World::WildEntry::getMinLevel)
    .addFunction("getMaxLevel", &World::WildEntry::getMaxLevel)
    .addFunction("getSpecies", &World::WildEntry::getSpecies)
    .endClass()

    .beginNamespace("bot")
    .addFunction("queue", &Bot::queueWrapper)
    .addFunction("clear", &Bot::clearWrapper)
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
    .addFunction("waitUntil", &Action::Wait::createFunc)
    .addFunction("lua", &Action::LuaClass::create)
    .endNamespace()

    .beginNamespace("action")
    .beginClass<AAction>("AAction")
    .addFunction("queue", &AAction::queue)
    .addFunction("getState", &AAction::getState)
    .addFunction("getCounter", &AAction::getCounter)
    .endClass()
    .deriveClass<Action::MoveTo, AAction>("MoveTo").endClass()
    .deriveClass<Action::TalkTo, AAction>("TalkTo").endClass()
    .deriveClass<Action::UseWarp, AAction>("UseWarp").endClass()
    .deriveClass<Action::PressButton, AAction>("PressButton").endClass()
    .deriveClass<Action::Wait, AAction>("Wait").endClass()
    .deriveClass<Action::LuaClass, AAction>("LuaClass").endClass()
    .endNamespace()

    .beginNamespace("script")
    .addFunction("getStd", &Script::getStd)
    .addFunction("getPerson", &Script::getPerson)
    .addFunction("getSign", &Script::getSign)
    .addFunction("getScript", &Script::getScript)
    .addFunction("getGeneric", &Script::getGenericScript)
    .addFunction("getLocalPerson", &Script::getLocalPerson)
    .addFunction("getLocalSign", &Script::getLocalSign)
    .addFunction("getLocalScript", &Script::getLocalScript)
    .addFunction("getLocalGeneric", &Script::getLocalGeneric)

    .beginClass<Script>("Script")
    .addConstructor<void (*)()>()
    .addFunction("load", &Script::load)
    .addFunction("loadStd", &Script::loadStd)
    .addFunction("print", &Script::print)
    .addFunction("getBank", &Script::getBank)
    .addFunction("getMap", &Script::getMap)
    .addFunction("getId", &Script::getId)
    .addFunction("getType", &Script::getType)
    .addFunction("getNbChoices", &Script::getNbChoices)
    .addFunction("getChoice", &Script::getChoice)
    .endClass()

    .beginClass<ChoicePts>("ChoicePts")
    .addFunction("getNbPoints", &ChoicePts::getNbPoints)
    .endClass()
    .endNamespace()

    .beginNamespace("config")
    .addFunction("getString", &Config::getString)
    .addFunction("setString", &Config::setString)
    .addFunction("getNumber", &Config::getNumber)
    .addFunction("setNumber", &Config::setNumber)
    .endNamespace()
    ;

  _initButtons();
  _initStates();
  _initWildTypes();
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

void            Lua::_initWildTypes()
{
  // Get global namespace
  lua_getglobal(_state, "_G");
  // Create "wild" namespace
  lua_pushstring(_state, "wild");
  lua_newtable(_state);

  // Push variables
  _pushvar("grass", WILD_GRASS);
  _pushvar("water", WILD_WATER);
  _pushvar("rock", WILD_ROCK);
  _pushvar("fishing", WILD_FISHING);

  lua_settable(_state, -3);
}

void            Lua::_error(int ret)
{
  const char    *str;

  if (ret && (str = lua_tostring(_state, -1)))
    {
      std::cerr << "Error: " << str << std::endl;
      lua_pop(_state, 1);
    }
}

void            Lua::doFile(const std::string &s)
{
  _error(luaL_dofile(_state, s.c_str()));
}

void            Lua::_printTop()
{
  switch (lua_type(_state, -1))
    {
    case LUA_TNIL:
      printf("nil\n");
      break;
    case LUA_TSTRING:
    case LUA_TNUMBER:
      printf("%s\n", lua_tostring(_state, -1));
      break;
    case LUA_TBOOLEAN:
      printf("%s\n", lua_toboolean(_state, -1) ? "true" : "false");
      break;
    case LUA_TTABLE:
      printf("table: %p\n", lua_topointer(_state, -1));
      break;
    case LUA_TFUNCTION:
      printf("function: %p\n", lua_topointer(_state, -1));
      break;
    case LUA_TUSERDATA:
      printf("userdata: %p\n", lua_topointer(_state, -1));
      break;
    case LUA_TTHREAD:
      printf("thread: %p\n", lua_topointer(_state, -1));
      break;
    case LUA_TLIGHTUSERDATA:
      printf("lightuserdata: %p\n", lua_topointer(_state, -1));
      break;
    }
}

void            Lua::doString(const std::string &s)
{
  int           old = lua_gettop(_state);

  _error(luaL_dostring(_state, s.c_str()));
  if (lua_gettop(_state) != old)
    {
      _printTop();
      lua_pop(_state, 1);
    }
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
  std::string   s;
  char          buff[256];
  int           ret;

  std::cout << "Lua REPL for Pokebot. Hit ^D to exit" << std::endl;
  while (1)
    {
      std::cout << "> " << std::flush;
      if ((ret = read(0, buff, 256)) <= 0)
        break;
      if (buff[ret - 1] == '\n')
        buff[ret - 1] = '\0';
      s = std::string(buff);
      if (s[0] == '=')
        s.replace(0, 1, "return ");
      doString(s);
    }
}
