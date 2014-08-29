#ifndef		__ACTION_MOVECURSOR_HH__
#define		__ACTION_MOVECURSOR_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		MoveCursor : public AAction
  {
  public:
    MoveCursor(uint8_t w, uint8_t h, uint8_t dest, uint8_t (*f)());
    ~MoveCursor();

  private:
    void	_init();
    void	_update();

  private:
    uint8_t	_w;
    uint8_t	_h;
    uint8_t	_dx;
    uint8_t	_dy;
    uint8_t	(*_f)();
  };
};

#endif
