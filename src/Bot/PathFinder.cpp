#include	"PathFinder.hh"

PathFinder::PathFinder(World::Map &m)
  : _m(m), _ows(Action::data->overWorlds()), _data(*Action::data)
{
  _walkableTiles = {0x0C, 0x00, 0x10};
  _hills = {
    Hill( 0, 1, 0x3B),	// Down
    Hill( 1, 0, 0x38),	// Right
    Hill(-1, 0, 0x39)	// Left
  };
  _movementCost[0x0202] = 30;
}

PathFinder::~PathFinder()
{
}

bool		PathFinder::_checkHills(int x, int y, World::Map::Node &next, World::Map::Node &curr)
{
  for (uint16_t i = 0; i < _hills.size(); i++)
    {
      if (x == _hills[i].x && y == _hills[i].y &&
	  (_hills[i].behavior == next.attr->behavior || _hills[i].behavior == curr.attr->behavior))
	return (true);
    }
  return (false);
}

bool		PathFinder::_checkWalkable(World::Map::Node &n)
{
  // Check walkable tiles (grass/tile near escalator, for now)
  return (std::find(_walkableTiles.begin(), _walkableTiles.end(), n.status) != _walkableTiles.end() &&
	  // Check that it's not an escalator
	  n.attr->behavior != 0x6b && n.attr->behavior != 0x6a);
}

bool		PathFinder::_checkOverWorld(uint16_t x, uint16_t y)
{
  for (int i = 1; i < 16 && (_ows[i].getMap() || _ows[i].getBank()); i++)
    {
      if (_ows[i].getBank() == _data.player().getBank() &&
	  _ows[i].getMap() == _data.player().getMap() &&
	  _ows[i].getDestX() == x && _ows[i].getDestY() == y)
	return (true);
    }
  return (false);
}

uint8_t		PathFinder::_getMovementCost(World::Map::Node &next)
{
  std::map<uint16_t, uint8_t>::iterator	it;

  it = _movementCost.find(next.attr->behavior);
  if (it != _movementCost.end())
    return (it->second);
  return (10);
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
      for (int i = 0; i < 4; i++)
	{
	  // Boundaries check
	  int	dx = (i - 1) * !(i & 1);
	  int	dy = (i - 2) * (i & 1);
	  int	x = curr->x + dx;
	  int	y = curr->y + dy;
	  if (x < 0 || x >= (int) _m.width || y < 0 || y >= (int) _m.height)
	    continue;

	  // Tile type check
	  World::Map::Node		*next = &(_m.data[y][x]);
	  if (_checkOverWorld(x, y))
	    continue;
	  if (!(_checkHills(dx, dy, *next, _m.data[curr->y][curr->x]) ||
		_checkWalkable(*next) ||
		// Block access to hill from a lower level
		next->attr->behavior == 0x32 ||
		// Check if escalator is the final tile
		((next->attr->behavior == 0x6b || next->attr->behavior == 0x6a) &&
		 x == (int) xe && y == (int) ye)))
	    continue;

	  World::Path::iterator	it = std::find(closedset.begin(), closedset.end(), next);
	  uint32_t	g = curr->g + _getMovementCost(*next);
	  if (it != closedset.end() && g >= next->g)
	    continue;
	  if (it == closedset.end() || g < next->g)
	    {
	      next->from = curr;
	      next->setG(g);
	      next->setF(xe, ye);
	      if (std::find(openset.begin(), openset.end(), next) == openset.end())
		openset.push_back(next);
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
