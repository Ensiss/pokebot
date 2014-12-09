#ifndef		__ACTION_WAIT_HH__
#define		__ACTION_WAIT_HH__

#include        <LuaBridge.h>
#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

using namespace luabridge;

namespace	Action
{
  class		Wait : public AAction
  {
  public:
    static Wait *create(uint16_t frames) { return (new Wait(frames)); }
    static Wait *createFunc(LuaRef func) { return (new Wait(func)); }

  public:
    Wait(uint16_t frames);
    Wait(bool (*until)());
    Wait(LuaRef until);
    ~Wait();

  private:
    void	_init();
    void	_update();

  private:
    uint16_t	_frames;
    bool	(*_until)();
    LuaRef      _luaFunc;
    bool	_useFrames;
  };
};

#endif
