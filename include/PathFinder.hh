#ifndef		__PATHFINDER_HH__
#define		__PATHFINDER_HH__

#include	<vector>
#include	<algorithm>
#include	"World.hh"

class		PathFinder
{
public:
  PathFinder(World::Map &m);
  ~PathFinder();

public:
  World::Path	*search(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye);

protected:
  int		_getNextIndex(World::Path *set);
  World::Path	*_rebuildPath(World::Path *set, World::Map::Node *node);

protected:
  World::Map	&_m;
};

#endif
