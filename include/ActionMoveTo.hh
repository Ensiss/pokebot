#ifndef		__ACTION_MOVETO_HH__
#define		__ACTION_MOVETO_HH__

#include	"Data.hh"
#include	"IAction.hh"
#include	"Action.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		MoveTo : public IAction
  {
  public:
    MoveTo(uint16_t x, uint16_t y);
    ~MoveTo();

  public:
    void	update();

  public:
    World::Path	*getPath() const { return (_path); }

  private:
    void	_releaseKeys();

  private:
    Data	&_data;
    World::Path	*_path;
    uint16_t	_tx;
    uint16_t	_ty;
    uint16_t	_oldx;
    uint16_t	_oldy;
    uint8_t	_pathi;
  };
};

#endif
