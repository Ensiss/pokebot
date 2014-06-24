#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Team.hpp"

void		doLoop()
{
  Team		pTeam((uint64_t) (workRAM + 0x24284));
  Team		eTeam((uint64_t) (workRAM + 0x2402C));
  int		step = 0;

  while (emulating) {
    if (++step % 50 == 0)
      {
	pTeam.update();
	eTeam.update();
	printf("\033[2J\033[0;0H");
	for (int i = 0; i < 6; i++)
	  {
	    PokemonData	*poke = pTeam.list + i;

	    printf("%s's (%d xp) %s\tvs\t", poke->getOtName(), poke->getXP(), poke->getNick());
	    poke = eTeam.list + i;
	    printf("%s\n", poke->getNick());
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
