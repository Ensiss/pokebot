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
	  map.data = new uint8_t*[map.height]();
	  for (int y = 0; y < map.height; y++)
	    {
	      map.data[y] = new uint8_t[map.width]();
	      for (int x = 0; x <  map.width; x++)
		map.data[y][x] = d[y * map.width + x] >> 10;
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
