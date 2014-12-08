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
          if (header->connectPtr != 0)
            {
              map.nbConnects = ((uint32_t *) gbaMem(header->connectPtr))[0];
              map.connects = (Connection *) gbaMem(((uint32_t *) gbaMem(header->connectPtr))[1]);
            }
          else
            {
              map.nbConnects = 0;
              map.connects = NULL;
            }
	  map.scriptPtr = header->scriptPtr;
	  map.data = new Map::Node*[map.height]();
          map.loadName(header->labelId);
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

void            World::Map::loadName(uint8_t id)
{
  uint32_t      nameAddr = ((uint32_t *) gbaMem(0x083F1CAC))[id - 88];
  uint8_t       *namePtr = (uint8_t *) gbaMem(nameAddr);

  this->labelId = id;
  this->name = "";
  for (int i = 0; namePtr[i] != 0xFF; i++)
    this->name += pokeCharsetToAscii(namePtr[i]);
}
