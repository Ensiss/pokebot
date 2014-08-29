#ifndef		__ACTION_PRESSBUTTON_HH__
#define		__ACTION_PRESSBUTTON_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include	"Action.hh"
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
    bool	_first;
    EKey	_key;
  };
};

#endif
