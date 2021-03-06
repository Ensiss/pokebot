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
    .beginNamespace("pb")
    .addFunction("getOverWorld", &Data::getOverWorldWrapper)
    .addFunction("getSpecies", &Data::getSpeciesWrapper)
    .addFunction("getItem", &Data::getItemWrapper)
    .addFunction("getMove", &Data::getMoveWrapper)
    .addFunction("getTypeName", &Data::getTypeNameWrapper)
    .addFunction("getAbilityName", &Data::getAbilityNameWrapper)
    .addFunction("getPlayerPokemon", &Data::getPlayerPokemonWrapper)
    .addFunction("getEnemyPokemon", &Data::getEnemyPokemonWrapper)
    .addFunction("getBattler", &Data::getBattlerWrapper)
    .addFunction("getCurrentMap", &Data::getCurrentMap)
    .addFunction("getMap", &Data::getMap)
    .addFunction("getDamage", &potentialDamage)
    .addFunction("getChanceToHit", &chanceToHit)
    .addFunction("getPlayer", &Data::getPlayerWrapper)
    .addFunction("getBattleMenu", &Data::getBattleMenuWrapper)
    .addFunction("getBagMenu", &Data::getBagMenuWrapper)
    .addFunction("getStartMenu", &Data::getStartMenuWrapper)
    .addFunction("getBag", &Data::getBagWrapper)
    .addFunction("getBot", &Bot::getBotWrapper)
    .addFunction("isInBattle", &Data::isInBattleWrapper)
    .addFunction("setButton", (void (*)(int, bool)) &sdlSetButton)
    .addFunction("pressButton", (void (*)(int)) &sdlPressButton)
    .addFunction("releaseButton", (void (*)(int)) &sdlReleaseButton)
    .addFunction("getButton", (bool (*)(int)) &sdlGetButton)

    .beginClass<Bot>("Bot")
    .addFunction("queue", &Bot::queue)
    .addFunction("clear", &Bot::clear)
    .endClass()

    .beginClass<Player>("Player")
    .addFunction("getX", &Player::getX)
    .addFunction("getY", &Player::getY)
    .addFunction("getBankId", &Player::getBankId)
    .addFunction("getMapId", &Player::getMapId)
    .endClass()

    .beginClass<BattleMenu>("BattleMenu")
    .addFunction("isOpen", &BattleMenu::isOpen)
    .addFunction("getMenu", &BattleMenu::getMenu)
    .addFunction("getAttack", &BattleMenu::getAttack)
    .addFunction("getCursor", &BattleMenu::getCursor)
    .endClass()

    .beginClass<BagMenu>("BagMenu")
    .addFunction("isOpen", &BagMenu::isOpen)
    .addFunction("getPocket", &BagMenu::getPocket)
    .addFunction("getCursor", &BagMenu::getCursor)
    .addFunction("getScroll", &BagMenu::getScroll)
    .endClass()

    .beginClass<StartMenu>("StartMenu")
    .addFunction("isOpen", &StartMenu::isOpen)
    .addFunction("getCursor", &StartMenu::getCursor)
    .addFunction("getNbItems", &StartMenu::getNbItems)
    .addFunction("getItemIndex", &StartMenu::getItemIndex)
    .addFunction("getState", &StartMenu::getState)
    .endClass()

    .beginClass<Bag>("Bag")
    .addFunction("getPocket", &Bag::getPocket)
    .endClass()

    .beginClass<BagPocket>("BagPocket")
    .addFunction("getCapacity", &BagPocket::getCapacity)
    .addFunction("getItem", &BagPocket::getItem)
    .endClass()

    .beginClass<BagItem>("BagItem")
    .addFunction("getQuantity", &BagItem::getQuantity)
    .addFunction("getId", &BagItem::getId)
    .endClass()

    .beginClass<World::Map>("Map")
    .addFunction("getWidth", &World::Map::getWidth)
    .addFunction("getHeight", &World::Map::getHeight)
    .addFunction("getNbPersons", &World::Map::getNbPersons)
    .addFunction("getNbWarps", &World::Map::getNbWarps)
    .addFunction("getNbScripts", &World::Map::getNbScripts)
    .addFunction("getNbSigns", &World::Map::getNbSigns)
    .addFunction("getNbMapScripts", &World::Map::getNbMapScripts)
    .addFunction("getNbConnections", &World::Map::getNbConnections)
    .addFunction("getPerson", &World::Map::getPerson)
    .addFunction("getWarp", &World::Map::getWarp)
    .addFunction("getScript", &World::Map::getScript)
    .addFunction("getSign", &World::Map::getSign)
    .addFunction("getMapScript", &World::Map::getMapScript)
    .addFunction("getConnection", &World::Map::getConnection)
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
    .addFunction("getMoveId", &IPokeData::getMoveId)
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
    .addFunction("getSpeciesId", &PokemonData::getSpeciesId)
    .addFunction("getItemId", &PokemonData::getItemId)
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
    .addFunction("getSpeciesId", &BattleData::getSpeciesId)
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
    .addFunction("getMapId", &OverWorld::getMapId)
    .addFunction("getBankId", &OverWorld::getBankId)
    .addFunction("getSpawnX", &OverWorld::getSpawnX)
    .addFunction("getSpawnY", &OverWorld::getSpawnY)
    .endClass()

    .beginClass<Move>("Move")
    .addFunction("getPower", &Move::getPower)
    .addFunction("getTypeId", &Move::getTypeId)
    .addFunction("getAccuracy", &Move::getAccuracy)
    .addFunction("getPP", &Move::getPP)
    .addFunction("getEffectAccuracy", &Move::getEffectAccuracy)
    .addFunction("getTargetId", &Move::getTargetId)
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
    .addFunction("getTypeId", &Species::getTypeId)
    .addFunction("getAbilityId", &Species::getAbilityId)
    .addFunction("getItemId", &Species::getItemId)
    .addFunction("getName", &Species::getName)
    .endClass()

    .beginClass<Item>("Item")
    .addFunction("getName", &Item::getName)
    .addFunction("getId", &Item::getId)
    .addFunction("getPrice", &Item::getPrice)
    .addFunction("getHoldEffect", &Item::getHoldEffect)
    .addFunction("getParameter", &Item::getParameter)
    .addFunction("getMysteryValue", &Item::getMysteryValue)
    .addFunction("getPocket", &Item::getPocket)
    .addFunction("getType", &Item::getType)
    .addFunction("getFieldUsageCodePtr", &Item::getFieldUsageCodePtr)
    .addFunction("getBattleUsage", &Item::getBattleUsage)
    .addFunction("getBattleUsageCodePtr", &Item::getBattleUsageCodePtr)
    .addFunction("getExtraParameter", &Item::getExtraParameter)
    .endClass()

    .beginClass<World::Map::Node>("Node")
    .addFunction("setFrom", &World::Map::Node::setFrom)
    .addFunction("setG", &World::Map::Node::setG)
    .addFunction("setF", &World::Map::Node::setF)
    .addFunction("getFrom", &World::Map::Node::getFrom)
    .addFunction("getTile", &World::Map::Node::getTile)
    .addFunction("getStatus", &World::Map::Node::getStatus)
    .addFunction("getBehavior", &World::Map::Node::getBehavior)
    .addFunction("getBackground", &World::Map::Node::getBackground)
    .addFunction("getX", &World::Map::Node::getX)
    .addFunction("getY", &World::Map::Node::getY)
    .addFunction("getF", &World::Map::Node::getF)
    .addFunction("getG", &World::Map::Node::getG)
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
    .addFunction("getMapId", &World::WarpEvt::getMapId)
    .addFunction("getBankId", &World::WarpEvt::getBankId)
    .endClass()

    .beginClass<World::PersonEvt>("Person")
    .addFunction("getEventNb", &World::PersonEvt::getEventNb)
    .addFunction("getX", &World::PersonEvt::getX)
    .addFunction("getY", &World::PersonEvt::getY)
    .addFunction("getLevel", &World::PersonEvt::getLevel)
    .addFunction("getMovementType", &World::PersonEvt::getMovementType)
    .addFunction("getMovement", &World::PersonEvt::getMovement)
    .addFunction("isVisible", &World::PersonEvt::isVisible)
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

    .beginClass<World::MapScript>("MapScript")
    .addFunction("getVariable", &World::MapScript::getVar)
    .addFunction("getValue", &World::MapScript::getValue)
    .addFunction("getScript", &World::MapScript::getScript)
    .addFunction("getType", &World::MapScript::getType)
    .addFunction("checksValue", &World::MapScript::checksValue)
    .endClass()

    .beginClass<World::Connection>("Connection")
    .addFunction("getType", &World::Connection::getType)
    .addFunction("getOffset", &World::Connection::getOffset)
    .addFunction("getBankId", &World::Connection::getBankId)
    .addFunction("getMapId", &World::Connection::getMapId)
    .endClass()

    .beginClass<World::Map::WildBattle>("WildBattle")
    .addFunction("getRatio", &World::Map::WildBattle::getRatio)
    .addFunction("getNbEntries", &World::Map::WildBattle::getNbEntries)
    .addFunction("getEntry", &World::Map::WildBattle::getEntry)
    .endClass()

    .beginClass<World::WildEntry>("WildEntry")
    .addFunction("getMinLevel", &World::WildEntry::getMinLevel)
    .addFunction("getMaxLevel", &World::WildEntry::getMaxLevel)
    .addFunction("getSpeciesId", &World::WildEntry::getSpeciesId)
    .endClass()

    .beginClass<Range>("Range")
    .addFunction("getMin", &Range::getMin)
    .addFunction("getMax", &Range::getMax)
    .endClass()

    .beginNamespace("mem")
    .addFunction("get8", &gbaMem<uint8_t>)
    .addFunction("get16", &gbaMem<uint16_t>)
    .addFunction("get32", &gbaMem<uint32_t>)
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
    .addFunction("getStartOffset", &Script::getStartOffset)
    .addFunction("getInstruction", &Script::getInstruction)
    .endClass()

    .beginClass<ChoicePts>("ChoicePts")
    .addFunction("getNbPoints", &ChoicePts::getNbPoints)
    .endClass()

    .beginClass<Script::Instruction>("Instruction")
    .addFunction("getOffset", &Script::Instruction::getOffset)
    .addFunction("getNext", &Script::Instruction::getNext)
    .addFunction("getCommand", &Script::Instruction::getCommand)
    .addFunction("isVisited", &Script::Instruction::isVisited)
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
  _initWildTypes();
  _initConnectionTypes();
  _initPocketTypes();
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
  _pushvar("l", KEY_BUTTON_L);
  _pushvar("r", KEY_BUTTON_R);
  _pushvar("count", KEY_BUTTON_R + 1);

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

void            Lua::_initConnectionTypes()
{
  // Get global namespace
  lua_getglobal(_state, "_G");
  // Create "wild" namespace
  lua_pushstring(_state, "connect");
  lua_newtable(_state);

  // Push variables
  _pushvar("none", CO_NONE);
  _pushvar("up", CO_UP);
  _pushvar("down", CO_DOWN);
  _pushvar("left", CO_LEFT);
  _pushvar("right", CO_RIGHT);
  _pushvar("dive", CO_DIVE);
  _pushvar("emerge", CO_EMERGE);

  lua_settable(_state, -3);
}

void            Lua::_initPocketTypes()
{
  // Get global namespace
  lua_getglobal(_state, "_G");
  // Create "wild" namespace
  lua_pushstring(_state, "pocketType");
  lua_newtable(_state);

  // Push variables
  _pushvar("main", POCKET_MAIN);
  _pushvar("keyItems", POCKET_KEY_ITEMS);
  _pushvar("balls", POCKET_BALLS);
  _pushvar("TMCase", POCKET_TM_CASE);
  _pushvar("berryPouch", POCKET_BERRY_POUCH);

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
