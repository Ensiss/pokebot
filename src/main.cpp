#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"

void		doLoop()
{
  Data		data;
  Team		&pTeam = data.playerTeam();
  Team		&eTeam = data.enemyTeam();
  int		step = 0;

  while (emulating) {
    if (++step % 40 == 0)
      {
	data.update();
	printf("\033[2J\033[0;0H");
	for (int i = 0; i < 6; i++)
	  {
	    printf("%s's (%d xp) %s", pTeam[i].getOtName(), pTeam[i].getXP(), data.name(pTeam[i].getSpecies()));
	    printf("\tvs\t%s\n", eTeam[i].getNick());
	  }
	World::Map	&m = data.world()[data.player().getBank()][data.player().getMap()];

	for (int y = 0; y < m.height; y++)
	  {
	    for (int x = 0; x < m.width; x++)
	      printf("\033[1;%dm%02x\033[0m ", m.getMatterColor(m[y][x], (y == data.player().getY() &&
									  x == data.player().getX())), m[y][x]);
	    printf("\n");
	  }
      }
    emulator.emuMain(emulator.emuCount);
    sdlPollEvents();
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
