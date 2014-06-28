#include	"World.hh"

World::World()
{
  int		banki = 0;
  uint32_t	*bankptr = (uint32_t *) (rom + BANK_PTR);
  uint32_t	rel = bankptr[banki];
  uint32_t	next = bankptr[banki + 1];

  while (next > ROM_OFFSET)
    {
      _banks.push_back(std::vector<Map>());
        std::vector<Map>	&maps = _banks.back();

      for (int mapi = rel; mapi < next; mapi += 4)
	{
	  maps.push_back(Map());
	  Map		&map = maps.back();
	  uint32_t	mapaddr = *((uint32_t *) (rom + mapi - ROM_OFFSET));
	  Header	*header = (Header *) (rom + mapaddr - ROM_OFFSET);
	  DataHeader	*dheader = (DataHeader *) (rom + header->mapPtr - ROM_OFFSET);
	  uint16_t	*d = (uint16_t *) (rom + dheader->data - ROM_OFFSET);

	  map.width = dheader->width;
	  map.height = dheader->height;
	  map.data = new Map::Node*[map.height]();
	  for (int y = 0; y < map.height; y++)
	    {
	      map.data[y] = new Map::Node[map.width]();
	      for (int x = 0; x <  map.width; x++)
		{
		  map.data[y][x] = Map::Node(x, y);
		  map.data[y][x].status = d[y * map.width + x] >> 10;
		}
	    }
	}
      banki++;
      rel = next;
      next = bankptr[banki + 1];
    }
}

World::~World()
{
}

int		World::Map::_getNextIndex(std::vector<Node*> *set)
{
  int		i = -1;
  uint32_t	min;
  uint32_t	count = 0;

  for (std::vector<Node*>::iterator it = set->begin(); it != set->end(); it++)
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

std::vector<World::Map::Node*>*	World::Map::_rebuildPath(std::vector<Node*>* set, Node *node)
{
  if (node->from)
    _rebuildPath(set, node->from);
  set->push_back(node);
  return (set);
}

std::vector<World::Map::Node*>*	World::Map::findPath(uint32_t xs, uint32_t ys, uint32_t xe, uint32_t ye)
{
  std::vector<Node*>	openset;
  std::vector<Node*>	closedset;

  openset.push_back(new Node(xs, ys));
  openset.back()->setF(xe, ye);
  while (openset.size())
    {
      int		i = _getNextIndex(&openset);
      Node*		curr = openset[i];
      openset.erase(openset.begin() + i);
      if (curr->x == xe && curr->y == ye)
	return (_rebuildPath(new std::vector<Node*>, curr));
      closedset.push_back(curr);
      for (int j = -1; j <= 1; j++)
	{
	  for (int i = -1; i <= 1; i++)
	    {
	      int	x = curr->x + i;
	      int	y = curr->y + j;
	      if (x < 0 || x >= width || y < 0 || y >= height ||
		  (!i && !j) || (i && j) || data[y][x].status != 0x0C)
		continue;
	      Node	*neighbor = &(data[y][x]);
	      std::vector<Node*>::iterator	it = std::find(closedset.begin(), closedset.end(), neighbor);
	      uint32_t	g = curr->g + (!i || !j ? 10 : 14);
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
