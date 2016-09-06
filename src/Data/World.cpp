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
          uint32_t	mapaddr = gbaMem<uint32_t>(mapi);
          Header	*header = gbaPtr<Header *>(mapaddr);
          DataHeader	*dheader = gbaPtr<DataHeader *>(header->mapPtr);
          TilesetHeader	*global = gbaPtr<TilesetHeader *>(dheader->globalTileset);
          TilesetHeader	*local = gbaPtr<TilesetHeader *>(dheader->localTileset);
          Map::TileAttr	*globPtr = gbaPtr<Map::TileAttr *>(global->behaviorPtr);
          Map::TileAttr	*localPtr = gbaPtr<Map::TileAttr *>(local->behaviorPtr);
          uint16_t	*d = gbaPtr<uint16_t *>(dheader->data);
          Event		*evtPtr = gbaPtr<Event *>(header->evtPtr);

          map.width = dheader->width;
          map.height = dheader->height;
          map.nbPersons = evtPtr->nbPersons;
          map.nbWarps = evtPtr->nbWarps;
          map.nbScripts = evtPtr->nbScripts;
          map.nbSigns = evtPtr->nbSigns;
          map.persons = gbaPtr<PersonEvt *>(evtPtr->personsPtr);
          map.warps = gbaPtr<WarpEvt *>(evtPtr->warpsPtr);
          map.scripts = gbaPtr<ScriptEvt *>(evtPtr->scriptsPtr);
          map.signs = gbaPtr<SignEvt *>(evtPtr->signsPtr);
          if (header->connectPtr != 0)
            {
              map.nbConnects = gbaMem<uint32_t>(header->connectPtr);
              map.connects = gbaPtr<Connection *>(gbaPtr<uint32_t *>(header->connectPtr)[1]);
            }
          else
            {
              map.nbConnects = 0;
              map.connects = NULL;
            }

          // Load map scripts
          MapScriptHeader *msHeaders = gbaPtr<MapScriptHeader *>(header->scriptPtr);
          for (int i = 0; msHeaders[i].type; i++)
              map.mapScripts.push_back(MapScript(msHeaders[i]));

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
  WildHeader	*wh = gbaPtr<WildHeader *>(0x083c9cb8);

  while (wh->bank != 0xFF || wh->map != 0xFF)
    {
      Map	&map = getMap(wh->bank, wh->map);
      for (int i = 0; i < 4; i++)
        {
          map.wildBattles[i].nbEntries = 0;
          map.wildBattles[i].ratio = 0;
          if (wh->entryPtr[i])
            {
              uint32_t	*eh = gbaPtr<uint32_t *>(wh->entryPtr[i]);
              map.wildBattles[i].ratio = eh[0];
              map.wildBattles[i].entries = gbaPtr<WildEntry *>(eh[1]);
              map.wildBattles[i].nbEntries = (wh->entryPtr[i] - eh[1]) / 4;
            }
        }
      wh++;
    }
}

void            World::Map::loadName(uint8_t id)
{
  uint32_t      nameAddr = gbaPtr<uint32_t *>(0x083F1CAC)[id - 88];
  uint8_t       *namePtr = gbaPtr<uint8_t *>(nameAddr);

  this->labelId = id;
  this->name = "";
  for (int i = 0; namePtr[i] != 0xFF; i++)
    this->name += pokeCharsetToAscii(namePtr[i]);
}
