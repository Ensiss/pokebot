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
  Team		&eTeam = data.playerTeam();
  int		step = 0;

  while (emulating) {
    if (++step % 50 == 0)
      {
	data.update();
	printf("\033[2J\033[0;0H");
	for (int i = 0; i < 6; i++)
	  {
	    printf("%s's (%d xp) %s", pTeam[i].getOtName(), pTeam[i].getXP(), data.name(pTeam[i].getSpecies()));
	    printf("\tvs\t%s\n", eTeam[i].getNick());
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
