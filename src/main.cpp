#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/gba/armdis.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"
#include	"Action.hh"
#include	"ActionMoveTo.hh"
#include	"PokeScript.hh"
#include	"ThumbDisas.hh"
#include	"PrintUtils.hh"
#include	<list>

Data		*Action::data = NULL;

void		doLoop()
{
  Action::MoveTo	*mvt = NULL;
  Data			&data = *Action::data;
  int			step = 0;

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
	  if (step == 900)
	    mvt = new Action::MoveTo(data.player().getX() + 7, data.player().getY());
	  mvt->update();
	  if (step % 20 == 0)
	    {
	      printf("\033[2J\033[0;0H");
	      printTeam(data);
	    }
	}

      emulator.emuMain(emulator.emuCount);
      sdlPollEvents(data);
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
  Action::data = new Data();
  doLoop();
  destroyVBAM();
  return (0);
}
