#include	"PrintUtils.hh"

void		printTeam(Data &data)
{
  Team		&pTeam = data.playerTeam();
  Team		&eTeam = data.enemyTeam();
  const BattleData	&pb = data.battlers()[0];
  const BattleData	&eb = data.battlers()[1];

  for (int i = 0; i < 6; i++)
    {
      const PokemonData	&p = pTeam[i];
      const PokemonData	&e = eTeam[i];
      const Species	&sp = data.species(p.getSpecies());

      if (!p.getSpecies() && !e.getSpecies())
	continue;
      printf("%s's %s", p.getOtName(), sp.getName());
      printf("(%s", data.type(sp.getType(0)));
      if (sp.getType(1) != sp.getType(0))
	printf("/%s", data.type(sp.getType(1)));
      printf(")\tvs\t%s (%d/%d hp)\n", data.species(e.getSpecies()).getName(), e.getHP(), e.getMaxHP());
      for (int m = 0; m < 4; m++)
	{
	  if (p.getMove(m))
	    {
	      const Move	&move = data.move(p.getMove(m));
	      const IPokeData	&idata = i ? p : (const IPokeData &) pb;
	      Range		dmg = data.potentialDamage(idata, eb, move);
	      int		acc = data.chanceToHit(idata, eb, move);

	      printf("\tMove %d: %s(%s) -> ", m, move.getName(), data.type(move.getType()));
	      printf(" (Accuracy: %d%%, %d/%d PP",
		     acc, p.getPP(m), move.getPP());
	      printf(", Potential dmg: %d-%d)\n", dmg.min, dmg.max);
	    }
	}
    }
}

void		printMap(Data &data)
{
  Player	&p = data.player();
  World::Map	&m = data.world()[p.getBank()][p.getMap()];

  for (int y = 0; y < m.height; y++)
    {
      for (int x = 0; x < m.width; x++)
	{
	  bool	node = false;
	  bool	ppos = (y == p.getY() && x == p.getX());
	  for (int a = 0; !node && a < m.nbWarps; a++)
	    if (m.warps[a].x == x && m.warps[a].y == y)
	      node = true;
	  printf("\033[1;%d;%dm%02x \033[0m", node && !ppos ? 47 : 40,
		 m.getMatterColor(m[y][x].status, ppos), m[y][x].status);
	}
      printf("\n");
    }
}

void		printWildPokemons(Data &data)
{
  Player	&p = data.player();
  World::Map	&m = data.world()[p.getBank()][p.getMap()];

  for (int i = 0; i < 4; i++)
    {
      World::Map::WildBattle	*wb = m.wildBattles + i;

      if (wb->nbEntries)
	{
	  printf("Type: %s", "Grass\0    Water\0    RockSmash\0Fishing" + 10 * i);
	  printf(" -> %d/255 chances to encounter a pokemon (%d%%)\n", wb->ratio, 100 * wb->ratio / 255);
	  for (int j = 0; j < wb->nbEntries; j++)
	    {
	      printf("\t%-10s lvl ", data.species(wb->entries[j].species).getName());
	      if (wb->entries[j].maxLvl != wb->entries[j].minLvl)
		printf("%d-", wb->entries[j].minLvl);
	      printf("%d\n", wb->entries[j].maxLvl);
	    }
	}
    }
}

void		printMenu(Data &data)
{
  printf("Main menu: %d\n", *((uint8_t *) gbaMem(0x0203ADE6))); // or 0x020370F4
  if (data.battleMenu().isOpen())
    {
      uint8_t	menu = data.battleMenu().getMenu();
      if (menu == 0)
	printf("Main battle menu, cursor on %d\n", data.battleMenu().getCursor());
      else if (menu == 1)
	printf("Attack menu, cursor on %d\n", data.battleMenu().getAttack());
      else if (menu == 2)
	printf("Bag menu, pocket %d, item %d\n", data.bagMenu().getPocket(), data.bagMenu().getItem());
      else if (menu == 3)
	printf("Pokemon switch menu\n");
    }
  else if (data.bagMenu().isOpen())
    printf("Bag pocket #%d open, cursor on item #%d\n", data.bagMenu().getPocket(), data.bagMenu().getItem());
}

void		printRAM(Data &data, uint32_t address, uint32_t sz, int linesz)
{
  static uint8_t	*old = new uint8_t[sz]();
  uint8_t		*p = (uint8_t *) gbaMem(address);

  for (int i = 0; i < sz; i++)
    {
      if (linesz && !(i % linesz))
	printf("\n");
      if (p[i] != old[i])
	{
	  printf("\033[31m");
	  old[i] = p[i];
	}
      printf("%02x ", p[i]);
      printf("\033[0m");
    }
  printf("\n");
}

void		printMessageBoxes(int offsetx, int offsety)
{
  uint8_t	*d = (uint8_t *) gbaMem(0x020204B4);
  for (int a = 0; a < 32 && d[12 * a] != 0xFF; a++)
    {
      int x = d[12 * a + 1];
      int y = d[12 * a + 2];
      for (int j = 0; j < d[12 * a + 4]; j++)
	{
	  printf("\033[%d;%dH", offsety + y + j, offsetx + 2 * x);
	  for (int i = 0; i < d[12 * a + 3]; i++)
	    printf("%2d", a);
	  printf("\n");
	}
    }
  printf("\n");
}

void		printString(uint32_t addr, uint32_t size)
{
  uint8_t	*d = (uint8_t *) gbaMem(addr);

  printf("%#x: '", addr);
  for (int i = 0; d[i] != 0xFF && i < size; i++)
    printf("%c", pokeCharsetToAscii(d[i]));
  printf("'\n");
}

void		searchString(uint32_t start, uint32_t size, const char *m)
{
  uint8_t	*d = (uint8_t *) gbaMem(start);

  for (int j = 0; j < size; j++)
    {
      int	i = -1;

      while (m[++i] && pokeCharsetToAscii(d[j + i]) == m[i]);
      if (!m[i])
	printString(start + j - 10, 50);
    }
}
