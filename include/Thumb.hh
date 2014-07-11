#ifndef		__THUMB_HH__
#define		__THUMB_HH__

#include	<cstdio>
#include	<stdint.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/gba/armdis.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Thumb
{
  void		disass(uint32_t addr, uint8_t rec = 0);
  void		disassPokeScript(uint8_t instr);
};

#endif
