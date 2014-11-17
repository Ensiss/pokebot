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
    MoveTo(uint16_t x, uint16_t y, bool allowApprox = false);
    MoveTo(uint8_t tid);
    ~MoveTo();

  private:
    void	_init();
    void	_update();

  public:
    void	addListener(const std::string &signal, void (Action::MoveTo::*listener)());
    World::Path	*getPath() const { return (_path); }

  private:
    void	_releaseKeys();
    void	_checkNPCMovement();
    void        _updateTargetPos();

  private:
    OverWorld	_oldow[16];
    bool	_owInit;
    bool        _approx;
    uint16_t	_tx;
    uint16_t	_ty;
    int         _tid;
    uint16_t	_oldx;
    uint16_t	_oldy;
    World::Path	*_path;
    uint8_t	_pathi;
  };
};

#endif
