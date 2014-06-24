#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

int		main(int ac, char **av)
{
  int		i = 0;

  if (ac < 2)
    {
      std::cerr << "Usage: ./pokebot <Pokemon_FireRed.gba>" << std::endl;
      return (1);
    }
  initVBAM(ac, av);
  while (emulating) {
    emulator.emuMain(emulator.emuCount);
    sdlPollEvents();
    sdlSetButton(KEY_BUTTON_A, ++i % 2);
  }
  destroyVBAM();
  return (0);
}
