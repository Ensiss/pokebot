#ifndef         __ACTION_CHANGEMAP_HH__
#define         __ACTION_CHANGEMAP_HH__

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
  class		ChangeMap : public AAction
  {
  public:
    static ChangeMap    *create(uint8_t t) { return (new ChangeMap((ConnectType) t)); }

  public:
    ChangeMap(ConnectType t);
    ~ChangeMap();

  private:
    void	_init();
    void	_update();

  private:
    ConnectType _type;
  };
};

#endif
