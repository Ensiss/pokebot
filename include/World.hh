#ifndef		__WORLD_HH__
#define		__WORLD_HH__

#include	<cstdio>
#include	<stdint.h>
#include	<vector>
#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"

enum	{
  ROCK=35,
  HILL=35,
  LADDER=34,
  WATER=36,
  ROAD=32,
  WALL=30,
  PLAYER=31
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

public:
  struct	Map
  {
    uint64_t	width;
    uint64_t	height;
    uint8_t	**data;

    uint8_t	*operator[](uint8_t y) { return (data[y]); }
    inline uint8_t	getMatterColor(uint8_t matter, bool pos) {
      return (pos ? PLAYER:
	      !matter ? LADDER :
	      matter == 1 ? WALL :
	      matter == 4 ? WATER :
	      matter == 12 ? ROAD :
	      matter == 13 ? HILL :
	      matter == 16 ? ROCK : 0);
    }
  };

public:
  World();
  ~World();

public:
  std::vector<Map>	&operator[](uint8_t bank) { return (_banks[bank]); }
  Map		&getMap(uint8_t bank, uint8_t map) { return (_banks[bank][map]); }

public:
  std::vector<std::vector<Map> >	_banks;
};

#endif
