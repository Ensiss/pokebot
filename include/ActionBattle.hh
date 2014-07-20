#ifndef		__ACTION_BATTLE_HH__
#define		__ACTION_BATTLE_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"Action.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		Battle : public AAction
  {
  public:
    Battle();
    ~Battle();

  private:
    void	_releaseKeys();
    uint8_t	_getBestMove();
    void	_selectMove(uint8_t move, uint8_t curr);

  public:
    void	update();

  private:
    Data	&_data;
    uint8_t	_tick;
  };
};

#endif
