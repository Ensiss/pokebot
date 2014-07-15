#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/gba/armdis.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"
#include	"Action.hh"
#include	"PokeScript.hh"
#include	"Thumb.hh"
#include	<list>

void		printTeam(Data &data)
{
  Team		&pTeam = data.playerTeam();
  Team		&eTeam = data.enemyTeam();

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
	      Range		dmg = data.potentialDamage(p, eTeam[0], move);

	      printf("\tMove %d: %s(%s) -> ", m, move.getName(), data.type(move.getType()));
	      printf(" (Power: %d, Accuracy: %d, %d/%d PP",
		     move.getPower(), move.getAccuracy(), p.getPP(m), move.getPP());
	      printf(", Potential dmg: %d-%d)\n", dmg.min, dmg.max);
	    }
	}
    }
}

void		printMap(Data &data, Action &action)
{
  Player	&p = data.player();
  World::Map	&m = data.world()[p.getBank()][p.getMap()];
  World::Path	*path = action.movement().getPath();

  for (int y = 0; y < m.height; y++)
    {
      for (int x = 0; x < m.width; x++)
	{
	  bool	node = false;
	  bool	ppos = (y == p.getY() && x == p.getX());
	  for (int a = 0; path && !node && a < path->size(); a++)
	    if (x == (*path)[a]->x && y == (*path)[a]->y)
	      node = true;
	  for (int a = 0; !node && a < m.nbWarps; a++)
	    if (m.warps[a].x == x && m.warps[a].y == y)
	      node = true;
	  printf("\033[1;%d;%dm%02x \033[0m", node && !ppos ? 47 : 40,
		 m.getMatterColor(m[y][x].status, ppos), m[y][x].status);
	}
      printf("\n");
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

void		printRAM(Data &data, uint32_t address, uint32_t sz)
{
  static uint8_t	*old = new uint8_t[sz]();
  uint8_t		*p = (uint8_t *) gbaMem(address - (sz / 2));

  for (int i = 0; i < sz; i++)
    {
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

void		doLoop()
{
  Data		data;
  Action	action(data);
  int		step = 0;

  while (emulating)
    {
      if (++step < 900)
	{
	  sdlSetButton(KEY_BUTTON_AUTO_A, step < 899);
	  sdlSetButton(KEY_BUTTON_SPEED, step < 899);
	}
      else
	{
	  data.update();
	  action.update();
	  if (step % 20 == 0)
	    {
	      printf("\033[2J\033[0;0H");
	      printTeam(data);
	      printMap(data, action);
	      printMenu(data);
	    }
	}

      emulator.emuMain(emulator.emuCount);
      sdlPollEvents(data, action);
    }
}

int		main(int ac, char **av)
{
  if (ac < 2)
    {
      std::cerr << "Usage: ./pokebot <Pokemon_FireRed.gba>" << std::endl;
      return (1);
    }
  initVBAM(ac, av);
  doLoop();
  destroyVBAM();
  return (0);
}
