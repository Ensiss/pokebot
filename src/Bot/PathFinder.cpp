#include	"PathFinder.hh"

PathFinder::PathFinder(World::Map &m)
  : _m(m), _walkableTiles{0x0C, 0x00, 0x10}
{
}

PathFinder::~PathFinder()
{
}

bool		checkHills(int i, int j, uint16_t next_behavior,
			   uint16_t curr_behavior)
{
  // Jump down hill
  return ((j == 1 && next_behavior == 0x3b) ||
	  (j == 1 && curr_behavior == 0x3b) ||
	  // Jump right hill
	  (i == 1 && next_behavior == 0x38) ||
	  (i == 1 && curr_behavior == 0x38) ||
	  // Jump left hill
	  (i == -1 && next_behavior == 0x39) ||
	  (i == -1 && curr_behavior == 0x39));
}

bool		PathFinder::_checkWalkable(World::Map::Node &n)
{
  // Check walkable tiles (grass/tile near escalator, for now)
  return (std::find(_walkableTiles.begin(), _walkableTiles.end(), n.status) != _walkableTiles.end() &&
	  // Check that it's not an escalator
	  n.attr->behavior != 0x6b && n.attr->behavior != 0x6a);
}

World::Path	*PathFinder::search(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye)
{
  World::Path		openset;
  World::Path		closedset;

  openset.push_back(new World::Map::Node(xs, ys));
  openset.back()->setF(xe, ye);
  while (openset.size())
    {
      int		i = _getNextIndex(&openset);
      World::Map::Node*	curr = openset[i];
      openset.erase(openset.begin() + i);
      if (curr->x == xe && curr->y == ye)
	return (_rebuildPath(new World::Path, curr));
      closedset.push_back(curr);
      for (int j = -1; j <= 1; j++)
	{
	  for (int i = -1; i <= 1; i++)
	    {
	      int	x = curr->x + i;
	      int	y = curr->y + j;

	      // Boundaries check
	      if (x < 0 || x >= (int) _m.width || y < 0 || y >= (int) _m.height ||
		  (!i && !j) || (i && j))
		continue;
	      // Tile type check
	      if (!(checkHills(i, j, _m.data[y][x].attr->behavior, _m.data[curr->y][curr->x].attr->behavior) ||
		    _checkWalkable(_m.data[y][x]) ||
		    // Block access to hill from a lower level
		    _m.data[y][x].attr->behavior == 0x32 ||
		    // Check if escalator is the final tile
		    (!j && (_m.data[y][x].attr->behavior == 0x6b ||
			    _m.data[y][x].attr->behavior == 0x6a) && x == (int) xe && y == (int) ye)))
		continue;

	      World::Map::Node		*neighbor = &(_m.data[y][x]);
	      World::Path::iterator	it = std::find(closedset.begin(), closedset.end(), neighbor);
	      uint32_t	g = curr->g + 10;
	      if (_m.data[y][x].attr->behavior == 0x0202)
		g += 20; // Grass "fear"
	      if (it != closedset.end() && g >= neighbor->g)
		continue;
	      if (it == closedset.end() || g < neighbor->g)
		{
		  neighbor->from = curr;
		  neighbor->setG(g);
		  neighbor->setF(xe, ye);
		  if (std::find(openset.begin(), openset.end(), neighbor) == openset.end())
		    openset.push_back(neighbor);
		}
	    }
	}
    }
  return (NULL);
}

int		PathFinder::_getNextIndex(World::Path *set)
{
  int		i = -1;
  uint32_t	min;
  uint32_t	count = 0;

  for (World::Path::iterator it = set->begin(); it != set->end(); it++)
    {
      if (i == -1 || (*it)->f < min)
	{
	  i = count;
	  min = (*it)->f;
	}
      count++;
    }
  return (i);
}

World::Path	*PathFinder::_rebuildPath(World::Path *set, World::Map::Node *node)
{
  if (node->from)
    _rebuildPath(set, node->from);
  set->push_back(node);
  return (set);
}
