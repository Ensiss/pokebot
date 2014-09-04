#ifndef		__ACTION_MOVETO_HH__
#define		__ACTION_MOVETO_HH__

#include	<cstring>
#include	"Data.hh"
#include	"AAction.hh"
#include	"PathFinder.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		MoveTo : public AAction
  {
  public:
    MoveTo(uint16_t x, uint16_t y);
    ~MoveTo();

  private:
    void	_init();
    void	_update();
    void	_test();

  public:
    void	addListener(const std::string &signal, void (Action::MoveTo::*listener)());
    World::Path	*getPath() const { return (_path); }

  private:
    void	_releaseKeys();
    void	_checkNPCMovement();

  private:
    OverWorld	_oldow[16];
    bool	_owInit;
    uint16_t	_tx;
    uint16_t	_ty;
    uint16_t	_oldx;
    uint16_t	_oldy;
    World::Path	*_path;
    uint8_t	_pathi;
  };
};

#endif
