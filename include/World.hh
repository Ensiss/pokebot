#ifndef		__WORLD_HH__
#define		__WORLD_HH__

#include	<cmath>
#include	<cstdio>
#include	<stdint.h>
#include	<vector>
#include	<algorithm>
#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"

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

  struct	SignEvt
  {
    uint16_t	x;
    uint16_t	y;
    uint8_t	level;
    uint8_t	type;
    uint32_t	scriptPtr;
  };

  struct	WarpEvt
  {
    uint16_t	x;
    uint16_t	y;
    uint8_t	level;
    uint8_t	destWarp;
    uint8_t	destMap;
    uint8_t	destBank;
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
  };

  struct	Connection
  {
    uint32_t	type;
    uint32_t	offset;
    uint8_t	bank;
    uint8_t	map;
    uint16_t	padding;
  };

  struct	WildHeader
  {
    uint8_t	bank;
    uint8_t	map;
    uint16_t	padding;
    uint32_t	entryPtr[4];
  };

  struct	WildEntry
  {
    uint8_t	minLvl;
    uint8_t	maxLvl;
    uint16_t	species;
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
    };

    struct	WildBattle
    {
      uint8_t	ratio;
      WildEntry	*entries;
      uint8_t	nbEntries;
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

  private:
    int			_getNextIndex(std::vector<Map::Node*> *set);
    std::vector<Node*>	*_rebuildPath(std::vector<Map::Node*>* set, Node *node);

  public:
    std::vector<Node*>	*findPath(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye);
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
  Map		&getMap(uint8_t bank, uint8_t map) { return ((*this)[bank][(map < (*this)[bank].size()) * map]); }

public:
  std::vector<std::vector<Map> >	_banks;
};

#endif
