#ifndef		__ACTION_PRESSBUTTON_HH__
#define		__ACTION_PRESSBUTTON_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		PressButton : public AAction
  {
  public:
    PressButton(EKey key);
    ~PressButton();

  private:
    void	_init();
    void	_update();

  private:
    int         _i;
    EKey	_key;
  };
};

#endif
