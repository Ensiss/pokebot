#include	"World.hh"

World::World()
{
  _initWorld();
  _initWildBattles();
}

World::~World()
{
}

void		World::_initWorld()
{
  int		banki = 0;
  uint32_t	*bankptr = (uint32_t *) (rom + BANK_PTR);
  uint32_t	rel = bankptr[banki];
  uint32_t	next = bankptr[banki + 1];

  while (next > ROM_OFFSET)
    {
      _banks.push_back(std::vector<Map>());
      std::vector<Map>	&maps = _banks.back();

      for (uint32_t mapi = rel; mapi < next; mapi += 4)
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
	  map.scriptPtr = header->scriptPtr;
	  map.data = new Map::Node*[map.height]();
	  for (uint16_t y = 0; y < map.height; y++)
	    {
	      map.data[y] = new Map::Node[map.width]();
	      for (uint16_t x = 0; x < map.width; x++)
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

void		World::_initWildBattles()
{
  WildHeader	*wh = (WildHeader *) gbaMem(0x083c9cb8);

  while (wh->bank != 0xFF || wh->map != 0xFF)
    {
      Map	&map = getMap(wh->bank, wh->map);
      for (int i = 0; i < 4; i++)
	{
	  map.wildBattles[i].nbEntries = 0;
	  map.wildBattles[i].ratio = 0;
	  if (wh->entryPtr[i])
	    {
	      uint32_t	*eh = (uint32_t *) gbaMem(wh->entryPtr[i]);
	      map.wildBattles[i].ratio = eh[0];
	      map.wildBattles[i].entries = (WildEntry *) gbaMem(eh[1]);
	      map.wildBattles[i].nbEntries = (wh->entryPtr[i] - eh[1]) / 4;
	    }
	}
      wh++;
    }
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

bool		checkWalkableTiles(std::vector<uint8_t> walkableTiles,
				   uint8_t status, uint16_t behavior)
{
  // Check walkable tiles (grass/tile near escalator, for now)
  return (std::find(walkableTiles.begin(),
		    walkableTiles.end(),
		    status) != walkableTiles.end() &&
	  // Check that it's not an escalator
	  behavior != 0x6b &&
	  behavior != 0x6a);
}

std::vector<World::Map::Node*>*	World::Map::findPath(uint32_t xs, uint32_t ys,
						     uint32_t xe, uint32_t ye)
{
  std::vector<Node*>	openset;
  std::vector<Node*>	closedset;
  static const uint8_t        arr[] = {0x0C,	//Grass/Road
				       0x00,	//Area around objects
				       0x10};	//Escalators
  std::vector<uint8_t>        walkableTiles(arr, arr + sizeof(arr) / sizeof(arr[0]));

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
	      if (x < 0 || x >= (int) width || y < 0 || y >= (int) height ||
		  (!i && !j) || (i && j))
		continue;
	      // Tile type check
	      if (!(checkHills(i, j, data[y][x].attr->behavior, data[curr->y][curr->x].attr->behavior) ||
		    checkWalkableTiles(walkableTiles, data[y][x].status, data[y][x].attr->behavior) ||
		    // Block access to hill from a lower level
		    data[y][x].attr->behavior == 0x32 ||
		    // Check if escalator is the final tile
		    (!j && (data[y][x].attr->behavior == 0x6b ||
			    data[y][x].attr->behavior == 0x6a) && x == (int) xe && y == (int) ye)))
		continue;

	      Node	*neighbor = &(data[y][x]);
	      std::vector<Node*>::iterator	it = std::find(closedset.begin(),
							       closedset.end(),
							       neighbor);
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
