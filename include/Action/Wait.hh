#ifndef		__ACTION_WAIT_HH__
#define		__ACTION_WAIT_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		Wait : public AAction
  {
  public:
    static Wait *create(uint16_t frames) { return (new Wait(frames)); }

  public:
    Wait(uint16_t frames);
    Wait(bool (*until)());
    ~Wait();

  private:
    void	_init();
    void	_update();

  private:
    uint16_t	_frames;
    bool	(*_until)();
    bool	_useFrames;
  };
};

#endif
