#ifndef		__WORLD_HH__
#define		__WORLD_HH__

#include	<cmath>
#include	<cstdio>
#include	<stdint.h>
#include	<vector>
#include	<algorithm>
#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"

enum            WildType
  {
    WILD_GRASS,
    WILD_WATER,
    WILD_ROCK,
    WILD_FISHING
  };

enum            ConnectType
  {
    CO_NONE,
    CO_DOWN,
    CO_UP,
    CO_LEFT,
    CO_RIGHT,
    CO_DIVE,
    CO_EMERGE,
    CO_COUNT
  };

class		World
{
private:
  struct	Header
  {
    uint32_t	mapPtr;
    uint32_t	evtPtr;
    uint32_t	scriptPtr;
    uint32_t	connectPtr;
    uint16_t	musicId;
    uint16_t	mapId;
    uint8_t	labelId;
    uint8_t	flash;
    uint8_t	weather;
    uint8_t	type;
    uint16_t	unknown;
    uint8_t	showLabel;
    uint8_t	battleType;
  };

  struct	DataHeader
  {
    uint32_t	width;
    uint32_t	height;
    uint32_t	border;
    uint32_t	data;
    uint32_t	globalTileset;
    uint32_t	localTileset;
    uint8_t	borderW;
    uint8_t	borderH;
  };

  struct	TilesetHeader
  {
    uint8_t	compressed;
    uint8_t	primary;
    uint16_t	unknown;
    uint32_t	imgPtr;
    uint32_t	palettePtr;
    uint32_t	blocksPtr;
    uint32_t	animPtr;
    uint32_t	behaviorPtr;
  };

  struct	Event
  {
    uint8_t	nbPersons;
    uint8_t	nbWarps;
    uint8_t	nbScripts;
    uint8_t	nbSigns;
    uint32_t	personsPtr;
    uint32_t	warpsPtr;
    uint32_t	scriptsPtr;
    uint32_t	signsPtr;
  };

  struct	WildHeader
  {
    uint8_t	bank;
    uint8_t	map;
    uint16_t	padding;
    uint32_t	entryPtr[4];
  };

public:
  struct	SignEvt
  {
    uint16_t	x;
    uint16_t	y;
    uint8_t	level;
    uint8_t	type;
    uint32_t	scriptPtr;

    uint16_t	getX() const { return (x); }
    uint16_t	getY() const { return (y); }
    uint8_t	getLevel() const { return (level); }
    uint16_t	getType() const { return (type); }
    uint32_t	getScript() const { return (scriptPtr); }
  };

  struct	WarpEvt
  {
    uint16_t	x;
    uint16_t	y;
    uint8_t	level;
    uint8_t	destWarp;
    uint8_t	destMap;
    uint8_t	destBank;

    uint16_t	getX() const { return (x); }
    uint16_t	getY() const { return (y); }
    uint8_t	getLevel() const { return (level); }
    uint8_t	getWarp() const { return (destWarp); }
    uint8_t	getMap() const { return (destMap); }
    uint8_t	getBank() const { return (destBank); }
  };

  struct	PersonEvt
  {
    uint8_t	evtNb;
    uint8_t	pictureNb;
    uint16_t	unknown;
    uint16_t	x;
    uint16_t	y;
    uint8_t	level;
    uint8_t	mvtType;
    uint8_t	mvt;
    uint8_t	unknown2;
    uint8_t	trainer;
    uint8_t	unknown3;
    uint16_t	view;
    uint32_t	scriptPtr;
    uint16_t	id;
    uint16_t	unknown4;

    bool	isVisible() const
    {
      if (!id)
	return (true);
      return (!getFlag(id));
    }
    uint8_t     getEventNb() const { return (evtNb); }
    uint16_t	getX() const { return (x); }
    uint16_t	getY() const { return (y); }
    uint8_t	getLevel() const { return (level); }
    uint8_t	getMovementType() const { return (mvtType); }
    uint8_t	getMovement() const { return (mvt); }
    bool	isTrainer() const { return (trainer); }
    uint16_t	getView() const { return (view); }
    uint32_t	getScript() const { return (scriptPtr); }
    uint16_t	getId() const { return (id); }
  };

  struct	ScriptEvt
  {
    uint16_t	x;
    uint16_t	y;
    uint8_t	level;
    uint8_t	unknown;
    uint16_t	varNb;
    uint16_t	varVal;
    uint16_t	unknown2;
    uint32_t	scriptPtr;

    uint16_t	getX() const { return (x); }
    uint16_t	getY() const { return (y); }
    uint8_t	getLevel() const { return (level); }
    uint16_t	getVarNb() const { return (varNb); }
    uint16_t	getVarValue() const { return (varVal); }
    uint32_t	getScript() const { return (scriptPtr); }
  };

  struct	Connection
  {
    uint32_t	type;
    uint32_t	offset;
    uint8_t	bank;
    uint8_t	map;
    uint16_t	padding;

    uint32_t	getType() const { return (type); }
    uint32_t	getOffset() const { return (offset); }
    uint8_t	getBank() const { return (bank); }
    uint8_t	getMap() const { return (map); }
  };

  struct	WildEntry
  {
    uint8_t	minLvl;
    uint8_t	maxLvl;
    uint16_t	species;

    uint8_t	getMinLevel() const { return (minLvl); }
    uint8_t	getMaxLevel() const { return (maxLvl); }
    uint16_t	getSpecies() const { return (species); }
  };

public:
  struct	Map
  {
    struct	TileAttr
    {
      uint16_t	behavior;
      uint16_t	bg;
    };

    struct	Node
    {
      uint8_t	status;
      uint16_t	tile;
      TileAttr	*attr;
      Node	*from;
      uint32_t	g;
      uint32_t	f;
      uint32_t	x;
      uint32_t	y;

      Node(uint32_t px = 0, uint32_t py = 0) : status(0), from(NULL), g(0), f(0), x(px), y(py) {}
      void	setG(uint32_t pg) { g = pg; }
      void	setF(uint32_t xe, uint32_t ye) { f = g + 10 * sqrt(POW(xe - x) + POW(ye - y)); }

      uint8_t   getStatus() const { return (status); }
      uint16_t  getBehavior() const { return (attr->behavior); }
      uint16_t  getBackground() const { return (attr->bg); }
      uint32_t  getX() const { return (x); }
      uint32_t  getY() const { return (y); }
    };

    struct	WildBattle
    {
      uint8_t	ratio;
      WildEntry	*entries;
      uint8_t	nbEntries;

      uint8_t   getRatio() const { return (ratio); }
      uint8_t   getNbEntries() const { return (nbEntries); }
      const WildEntry   &getEntry(uint8_t id) const { return (entries[id]); }
    };

    enum
      {
	ROCK = 35,
	HILL = 35,
	LADDER = 34,
	WATER = 36,
	ROAD = 32,
	WALL = 30,
	PLAYER = 31
      };

    uint64_t	width;
    uint64_t	height;
    Node	**data;
    uint8_t	nbPersons;
    uint8_t	nbWarps;
    uint8_t	nbScripts;
    uint8_t	nbSigns;
    PersonEvt	*persons;
    WarpEvt	*warps;
    ScriptEvt	*scripts;
    SignEvt	*signs;
    uint32_t	nbConnects;
    Connection	*connects;
    uint32_t	scriptPtr;
    WildBattle	wildBattles[4];
    uint8_t     labelId;
    std::string name;

  public:
    // Getters
    uint64_t    getWidth() const { return (width); }
    uint64_t    getHeight() const { return (height); }
    const Node  &getNode(uint8_t x, uint8_t y) const { return (data[y][x]); }
    uint8_t     getNbPersons() const { return (nbPersons); }
    uint8_t     getNbWarps() const { return (nbWarps); }
    uint8_t     getNbScripts() const { return (nbScripts); }
    uint8_t     getNbSigns() const { return (nbSigns); }
    uint32_t    getNbConnections() const { return (nbConnects); }
    uint32_t    getScripPtr() const { return (scriptPtr); }
    uint8_t     getLabelId() const { return (labelId); }
    const PersonEvt     &getPerson(uint8_t id) const { return (persons[id]); }
    const WarpEvt       &getWarp(uint8_t id) const { return (warps[id]); }
    const ScriptEvt     &getScript(uint8_t id) const { return (scripts[id]); }
    const SignEvt       &getSign(uint8_t id) const { return (signs[id]); }
    const Connection    &getConnection(uint8_t id) const { return (connects[id]); }
    const WildBattle    &getWildBattle(uint8_t id) const { return (wildBattles[id]); }
    const std::string&  getName() const { return (name); }

  public:
    void                loadName(uint8_t id);
    Node		*operator[](uint8_t y) { return (data[y]); }
    inline uint8_t	getMatterColor(uint8_t matter, bool pos) {
      return (pos ? PLAYER:
	      !matter ? LADDER :
	      matter == 1 ? WALL :
	      matter == 4 ? WATER :
	      matter == 12 ? ROAD :
	      matter == 13 ? HILL :
	      matter == 16 ? LADDER : 0);
    }
  };
  typedef	std::vector<Map::Node*>	Path;

public:
  World();
  ~World();

private:
  void		_initWorld();
  void		_initWildBattles();

public:
  std::vector<Map>	&operator[](uint8_t bank) { return (_banks[(bank < _banks.size()) * bank]); }
  Map                   &getMap(uint8_t bank, uint8_t map) { return ((*this)[bank][(map < (*this)[bank].size()) * map]); }

public:
  std::vector<std::vector<Map> >	_banks;
};

#endif
