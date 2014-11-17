#ifndef         __ACTION_USEWARP_HH__
#define         __ACTION_USEWARP_HH__

#include        <map>
#include	"Data.hh"
#include	"AAction.hh"
#include        "PressButton.hh"
#include        "Wait.hh"
#include        "MoveTo.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		UseWarp : public AAction
  {
  public:
    UseWarp(uint8_t personId);
    ~UseWarp();

  private:
    void	_init();
    void	_update();
    void        _turnToOW();

  private:
    uint8_t     _id;
    uint8_t     _i;
    static std::map<uint16_t, EKey>     _behaviours;
  };
};

#endif
