#ifndef         __ACTION_USECONNECTION_HH__
#define         __ACTION_USECONNECTION_HH__

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
  class		UseConnection : public AAction
  {
  public:
    static UseConnection      *create(uint8_t dir) { return (new UseConnection(dir)); }

  public:
    UseConnection(uint8_t dir);
    ~UseConnection();

  private:
    void	_init();
    void	_update();

  private:
    uint8_t     _dir;
    uint8_t     _origBank;
    uint8_t     _origMap;
    EKey        _keyDir;
    uint8_t     _tx;
    uint8_t     _ty;
  };
};

#endif
