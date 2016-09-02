#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/gba/armdis.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include        "../vbam/sdl/debugger.h"
#include	"Data.hh"
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
              VM::vm->update();
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

static bool     isROMValid()
{
  const char *err = NULL;

  if (strncmp(gbaPtr<char *>(0x080000A0), "POKEMON FIRE", 12))
    err = "the loaded game is not Pokemon FireRed";
  else if (strncmp(gbaPtr<char *>(0x080000AC), "BPRE", 4))
    err = "the loaded game is not the english version";
  else if (gbaMem<uint8_t>(0x080000BC) != 0x00)
    err = "the game version is not V1.0";

  if (err)
    fprintf(stderr, "Error: %s. Check Readme.md to see what the right rom is\n", err);
  return !err;
}

int		main(int ac, char **av)
{
  if (ac < 2)
    {
      std::cerr << "Usage: ./pokebot <Pokemon_FireRed.gba>" << std::endl;
      return (1);
    }
  initVBAM(ac, av);

  if (isROMValid())
    {
      // Break when script engine PC is modified
      debuggerDoString("bpw 03000EB8 4");
      Data::data = new Data();
      VM::vm = new VM();
      L.init();
      doLoop();
    }
  destroyVBAM();
  return (0);
}
