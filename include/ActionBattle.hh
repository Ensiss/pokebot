#ifndef		__ACTION_BATTLE_HH__
#define		__ACTION_BATTLE_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"Action.hh"
#include	"ActionMoveCursor.hh"
#include	"ActionPressButton.hh"
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
    void	_init();
    void	_update();

  private:
    uint8_t	_getBestMove();
  };
};

#endif
