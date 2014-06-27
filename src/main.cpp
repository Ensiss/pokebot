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

  printf("%p\n", workRAM);
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
	uint8_t		bank = *((uint8_t *) (workRAM + 0x031DBC));
	uint8_t		map = *((uint8_t *) (workRAM + 0x031DBD));
	World::Map	&m = data.world().getMap(bank, map);
	uint32_t	offset = *((uint32_t *) (internalRAM + 0x05008)) - 0x2000000;
	uint16_t	px = *((uint16_t *) (workRAM + offset));
	uint16_t	py = *((uint16_t *) (workRAM + offset + 2));
	for (int y = 0; y < m.height; y++)
	  {
	    for (int x = 0; x < m.width; x++)
	      {
		if (x == px && y == py)
		  printf("\033[31m");
		printf("%02x ", m[y][x]);
		if (x == px && y == py)
		  printf("\033[0m");
	      }
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
