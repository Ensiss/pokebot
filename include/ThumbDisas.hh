#ifndef		__THUMB_HH__
#define		__THUMB_HH__

#include	<stdint.h>
#include	<cstdio>
#include	<vector>
#include	<queue>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/gba/armdis.h"
#include	"../vbam/sdl/SDLGlobals.h"

class		ThumbDisas
{
private:
  struct	Range
  {
    Range(uint32_t x = 0, uint32_t y = 0)
      : start(x), end(y) {}
    uint32_t	start;
    uint32_t	end;
  };

public:
  ThumbDisas();
  ~ThumbDisas();

private:
  void		_reset();
  bool		_setupNextAddr();

public:
  void		print(uint32_t addr);
  void		printInstr(uint8_t instr);
  void		printSpecial(uint16_t instr);

private:
  uint32_t		_offset;
  uint32_t		_start;
  uint32_t		_pc;
  uint32_t		_oldpc;
  std::vector<Range>	_ranges;
  std::queue<uint32_t>	_addrs;
};

#endif
