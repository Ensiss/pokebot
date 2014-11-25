#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/gba/armdis.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"
#include	"Action/AAction.hh"
#include	"Action/MoveTo.hh"
#include	"Action/TalkTo.hh"
#include	"Action/UseWarp.hh"
#include	"Action/Battle.hh"
#include	"Script.hh"
#include	"ThumbDisas.hh"
#include	"PrintUtils.hh"
#include	"Bot.hh"
#include        "VM.hh"
#include        "Lua.hh"

Data		*Action::data = NULL;
extern Lua      L;

void		doLoop()
{
  Data		&data = *Action::data;
  Bot		bot;
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
	  bot.update();
	  if (step == 900)
            bot.setBattleAction(new Action::Battle());
	  if (step % 20 == 0)
	    {
	      printf("\033[2J\033[0;0H");
	      printTeam(data);
	      printMap(data);
	    }
	}

      emulator.emuMain(emulator.emuCount);
      sdlPollEvents(data, bot);
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
  Script::initStd();
  L.init();
  doLoop();
  destroyVBAM();
  return (0);
}
