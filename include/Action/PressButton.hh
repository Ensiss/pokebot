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
    static PressButton  *create(EKey key) { return (new PressButton(key)); }

  public:
    PressButton(EKey key);
    ~PressButton();

  private:
    void	_init();
    void	_update();

  private:
    EKey	_key;
  };
};

#endif
