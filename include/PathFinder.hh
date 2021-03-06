#ifndef		__PATHFINDER_HH__
#define		__PATHFINDER_HH__

#include	<map>
#include	<vector>
#include	<algorithm>
#include	"World.hh"
#include        "Data.hh"

class		PathFinder
{
public:
  PathFinder(World::Map &m);
  ~PathFinder();

public:
  World::Path	*search(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye, uint8_t dist = 0);

protected:
  int		_getNextIndex(World::Path *set);
  World::Path	*_rebuildPath(World::Path *set, World::Map::Node *node);
  bool		_checkHills(int x, int y, World::Map::Node &next, World::Map::Node &curr);
  bool		_checkWalkable(World::Map::Node &n);
  bool		_checkOverWorld(uint16_t x, uint16_t y);
  uint8_t	_getMovementCost(World::Map::Node &next);

public:
  struct	Hill
  {
    uint8_t	x;
    uint8_t	y;
    uint16_t	behavior;

    Hill(uint8_t px, uint8_t py, uint16_t pb) : x(px), y(py), behavior(pb) {}
  };

protected:
  World::Map			&_m;
  const OverWorld		*_ows;
  Data				&_data;
  std::map<uint16_t, uint8_t>	_movementCost;
  std::vector<uint8_t>		_walkableTiles;
  std::vector<Hill>		_hills;
  std::vector<uint8_t>		_notMoving;
};

#endif
