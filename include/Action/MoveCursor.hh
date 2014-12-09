#ifndef		__ACTION_MOVECURSOR_HH__
#define		__ACTION_MOVECURSOR_HH__

#include        <LuaBridge.h>
#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

using namespace luabridge;

namespace	Action
{
  class		MoveCursor : public AAction
  {
  public:
    static MoveCursor   *create(uint8_t w, uint8_t h, uint8_t dest, LuaRef f) { return (new MoveCursor(w, h, dest, f)); }

  public:
    MoveCursor(uint8_t w, uint8_t h, uint8_t dest, uint8_t (*f)());
    MoveCursor(uint8_t w, uint8_t h, uint8_t dest, LuaRef func);
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
    LuaRef      _luaFunc;
  };
};

#endif
