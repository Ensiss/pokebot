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
#include        "Config.hh"

extern Lua      L;

void		doLoop()
{
  Data		&data = *Data::data;
  int		step = 0;

  while (emulating)
    {
      if (paused)
        {
          L.doREPL();
          paused = false;
        }
      L.doFunc("onEnterFrame");
      if (++step < 900)
	{
	  sdlSetButton(KEY_BUTTON_AUTO_A, step < 899);
	  sdlSetButton(KEY_BUTTON_SPEED, step < 899);
	}
      else
	{
	  data.update();
          Bot::bot.update();
	  if (step == 900)
            {
              Bot::bot.setBattleAction(new Action::Battle());
              L.doFunc("onInit");
            }
	  if (step % Config::getNumber("refreshRate") == 0)
	    {
              if (Config::getNumber("clearOnRefresh"))
                printf("\033[2J\033[0;0H");
	      printTeam(data);
	      printMap(data);
              L.doFunc("onRefresh");
	    }
	}

      emulator.emuMain(emulator.emuCount);
      sdlPollEvents(data, Bot::bot);
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
  Data::data = new Data();
  L.init();
  doLoop();
  destroyVBAM();
  return (0);
}
