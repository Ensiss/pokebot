#ifndef		__ACTION_TURNDIRECTION_HH__
#define		__ACTION_TURNDIRECTION_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include        "PressButton.hh"
#include        "Wait.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		TurnDirection : public AAction
  {
  public:
    static TurnDirection  *create(uint8_t key) { return (new TurnDirection((EKey) key)); }

  public:
    TurnDirection(EKey key);
    ~TurnDirection();

  private:
    void	_init();
    void	_update();

  private:
    EKey	_key;
  };
};

#endif
