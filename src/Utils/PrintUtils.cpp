#include	"PrintUtils.hh"

void		printTeam(Data &data)
{
  Team		&pTeam = data.getPlayerTeam();
  Team		&eTeam = data.getEnemyTeam();
  const BattleData	&pb = data.getBattlers()[0];
  const BattleData	&eb = data.getBattlers()[1];

  try
    {
      for (int i = 0; i < 6; i++)
        {
          const PokemonData	&p = pTeam[i];
          const PokemonData	&e = eTeam[i];
          const Species	&sp = data.getSpecies(p.getSpecies());

          if (!p.getSpecies() && !e.getSpecies())
            continue;
          printf("%s's %s", p.getOtName(), sp.getName());
          printf("(%s", data.getTypeName(sp.getType(0)));
          if (sp.getType(1) != sp.getType(0))
            printf("/%s", data.getTypeName(sp.getType(1)));
          printf(")\tvs\t%s (%d/%d hp)\n", data.getSpecies(e.getSpecies()).getName(), e.getHP(), e.getMaxHP());
          for (int m = 0; m < 4; m++)
            {
              if (p.getMove(m))
                {
                  const Move	&move = data.getMove(p.getMove(m));
                  const IPokeData	&idata = i ? p : (const IPokeData &) pb;
                  Range		dmg = data.potentialDamage(idata, eb, move);
                  int		acc = data.chanceToHit(idata, eb, move);

                  printf("\tMove %d: %s(%s) -> ", m, move.getName(), data.getTypeName(move.getType()));
                  printf(" (Accuracy: %d%%, %d/%d PP",
                         acc, p.getPP(m), move.getPP());
                  printf(", Potential dmg: %d-%d)\n", dmg.min, dmg.max);
                }
            }
        }
    }
  catch (std::out_of_range &e)
    {
      std::cout << e.what() << std::endl;
    }
}

void		printMap(Data &data, uint8_t flags)
{
  Player	&p = data.getPlayer();
  World::Map	&m = data.getWorld()[p.getBankId()][p.getMapId()];

  if (!flags)
    flags = M_STATUS;
  for (uint16_t y = 0; y < m.getHeight(); y++)
    {
      for (uint16_t x = 0; x < m.getWidth(); x++)
	{
	  bool	node = false;
	  bool	ppos = (y == p.getY() && x == p.getX());
	  for (int a = 0; !node && a < m.getNbWarps(); a++)
	    if (m.getWarp(a).getX() == x && m.getWarp(a).getY() == y)
	      node = true;
	  printf("\033[1;%d;%dm", node && !ppos ? 47 : 40,
		 m.getMatterColor(m.getNode(x, y).getStatus(), ppos));
	  if (flags & M_STATUS)
	    printf("%02x ", m.getNode(x, y).getStatus());
	  if (flags & M_BEHAVIOR)
	    printf("%02x ", m.getNode(x, y).getBehavior());
	  if (flags & M_BGROUND)
	    printf("%02x ", m.getNode(x, y).getBackground());
	  printf("\033[0m");
	}
      printf("\n");
    }
}

void		printWildPokemons(Data &data)
{
  Player	&p = data.getPlayer();
  World::Map	&m = data.getWorld()[p.getBankId()][p.getMapId()];

  for (int i = 0; i < 4; i++)
    {
      const World::Map::WildBattle &wb = m.getWildBattle(i);

      if (wb.getNbEntries())
	{
	  printf("Type: %s", "Grass\0    Water\0    RockSmash\0Fishing" + 10 * i);
	  printf(" -> %d/255 chances to encounter a pokemon (%d%%)\n", wb.getRatio(), 100 * wb.getRatio() / 255);
	  for (int j = 0; j < wb.getNbEntries(); j++)
	    {
              const World::WildEntry &entry = wb.getEntry(j);
	      printf("\t%-10s lvl ", data.getSpecies(entry.getSpecies()).getName());
	      if (entry.getMaxLevel() != entry.getMinLevel())
		printf("%d-", entry.getMinLevel());
	      printf("%d\n", entry.getMaxLevel());
	    }
	}
    }
}

void		printMenu(Data &data)
{
  printf("Main menu: %d\n", gbaMem<uint8_t>(0x0203ADE6)); // or 0x020370F4
  if (data.getBattleMenu().isOpen())
    {
      uint8_t	menu = data.getBattleMenu().getMenu();
      if (menu == 0)
	printf("Main battle menu, cursor on %d\n", data.getBattleMenu().getCursor());
      else if (menu == 1)
	printf("Attack menu, cursor on %d\n", data.getBattleMenu().getAttack());
      else if (menu == 2)
	printf("Bag menu, pocket %d, item %d\n", data.getBagMenu().getPocket(), data.getBagMenu().getItem());
      else if (menu == 3)
	printf("Pokemon switch menu\n");
    }
  else if (data.getBagMenu().isOpen())
    printf("Bag pocket #%d open, cursor on item #%d\n", data.getBagMenu().getPocket(), data.getBagMenu().getItem());
}

void		printRAM(Data &data, uint32_t address, uint32_t sz, int linesz)
{
  static uint8_t	*old = new uint8_t[sz]();
  uint8_t		*p = gbaPtr<uint8_t *>(address);

  for (uint32_t i = 0; i < sz; i++)
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
  uint8_t	*d = gbaPtr<uint8_t *>(0x020204B4);
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
  uint8_t	*d = gbaPtr<uint8_t *>(addr);

  printf("%#x: '", addr);
  for (uint32_t i = 0; d[i] != 0xFF && i < size; i++)
    printf("%c", pokeCharsetToAscii(d[i]));
  printf("'\n");
}

void		searchString(uint32_t start, uint32_t size, const char *m)
{
  uint8_t	*d = gbaPtr<uint8_t *>(start);

  for (uint32_t j = 0; j < size; j++)
    {
      int	i = -1;

      while (m[++i] && pokeCharsetToAscii(d[j + i]) == m[i]);
      if (!m[i])
	printString(start + j - 10, 50);
    }
}

void            printMultiChoices(Data &data)
{
  for (uint8_t mc = 0; mc <= 0x40; mc++)
    {
      const MultiChoice &choice = data.getMultiChoice(mc);
      printf("Choice #%d\n", mc);
      for (int stri = 0; stri < choice.getNbChoices(); stri++)
        {
          uint8_t *d = gbaPtr<uint8_t *>(choice.getChoicePtr(stri));
          printf("\t");
          for (uint32_t i = 0; d[i] != 0xFF; i++)
            printf("%c", pokeCharsetToAscii(d[i]));
          printf("\n");
        }
    }
}

void            printStdFunctions(Data &data)
{
  Script        sc;

  for (int i = 0; i < 10; i++)
    {
      printf("STD FUNCTION #%d @", i);
      printf("0x%08x\n", gbaPtr<uint32_t *>(0x08160450)[i]);
      sc.loadStd(i);
      sc.print();
    }
}
