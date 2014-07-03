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
	  uint32_t	mapaddr = *((uint32_t *) gbaMem(mapi));
	  Header	*header = (Header *) gbaMem(mapaddr);
	  DataHeader	*dheader = (DataHeader *) gbaMem(header->mapPtr);
	  TilesetHeader	*global = (TilesetHeader *) gbaMem(dheader->globalTileset);
	  TilesetHeader	*local = (TilesetHeader *) gbaMem(dheader->localTileset);
	  Map::TileAttr	*globPtr = (Map::TileAttr *) gbaMem(global->behaviorPtr);
	  Map::TileAttr	*localPtr = (Map::TileAttr *) gbaMem(local->behaviorPtr);
	  uint16_t	*d = (uint16_t *) gbaMem(dheader->data);
	  Event		*evtPtr = (Event *) gbaMem(header->evtPtr);

	  map.width = dheader->width;
	  map.height = dheader->height;
	  map.nbPersons = evtPtr->nbPersons;
	  map.nbWarps = evtPtr->nbWarps;
	  map.nbScripts = evtPtr->nbScripts;
	  map.nbSigns = evtPtr->nbSigns;
	  map.persons = (PersonEvt *) gbaMem(evtPtr->personsPtr);
	  map.warps = (WarpEvt *) gbaMem(evtPtr->warpsPtr);
	  map.scripts = (ScriptEvt *) gbaMem(evtPtr->scriptsPtr);
	  map.signs = (SignEvt *) gbaMem(evtPtr->signsPtr);
	  map.nbConnects = ((uint32_t *) gbaMem(header->connectPtr))[0];
	  map.connects = (Connection *) gbaMem(((uint32_t *) gbaMem(header->connectPtr))[1]);
	  map.data = new Map::Node*[map.height]();
	  for (int y = 0; y < map.height; y++)
	    {
	      map.data[y] = new Map::Node[map.width]();
	      for (int x = 0; x <  map.width; x++)
		{
		  uint16_t	t = d[y * map.width + x] & ((1 << 10) - 1);

		  map.data[y][x] = Map::Node(x, y);
		  map.data[y][x].status = d[y * map.width + x] >> 10;
		  map.data[y][x].tile = t;
		  map.data[y][x].attr = t < 0x280 ? globPtr + t : localPtr + t - 0x280;
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
	      // Boundaries check
	      if (x < 0 || x >= width || y < 0 || y >= height ||
		  (!i && !j) || (i && j))
		continue;
	      // Tile type check
	      if (!(data[y][x].status == 0x0C ||
		    (j == 1 && data[y][x].attr->behavior == 0x3b) ||
		    (j == 1 && data[curr->y][curr->x].attr->behavior == 0x3b)))
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
