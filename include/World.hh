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

public:
  struct	Map
  {
    struct	Node
    {
      uint8_t	status;
      Node	*from;
      uint32_t	g;
      uint32_t	f;
      uint32_t	x;
      uint32_t	y;

      Node(uint32_t px = 0, uint32_t py = 0) : status(0), from(NULL), g(0), f(0), x(px), y(py) {}
      void	setG(uint32_t pg) { g = pg; }
      void	setF(uint32_t xe, uint32_t ye) { f = g + 10 * sqrt(POW(xe - x) + POW(ye - y)); }
    };

    uint64_t	width;
    uint64_t	height;
    Node	**data;

  private:
    int			_getNextIndex(std::vector<Map::Node*> *set);
    std::vector<Node*>*	_rebuildPath(std::vector<Map::Node*>* set, Node *node);

  public:
    std::vector<Node*>*	findPath(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye);
    Node		*operator[](uint8_t y) { return (data[y]); }
  };

public:
  World();
  ~World();

public:
  std::vector<Map>	&operator[](uint8_t bank) { return (_banks[(bank < _banks.size()) * bank]); }
  Map		&getMap(uint8_t bank, uint8_t map) { return ((*this)[bank][(map < (*this)[bank].size()) * map]); }

public:
  std::vector<std::vector<Map> >	_banks;
};

#endif
