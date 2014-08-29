#ifndef		__ACTION_BATTLE_HH__
#define		__ACTION_BATTLE_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"MoveCursor.hh"
#include	"PressButton.hh"
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
    void	_attack(uint8_t atk);
    void	_run();

  private:
    uint8_t	_getBestMove();
  };
};

#endif
