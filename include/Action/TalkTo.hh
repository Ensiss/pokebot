#ifndef         __ACTION_TALKTO_HH__
#define         __ACTION_TALKTO_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include        "PressButton.hh"
#include        "Wait.hh"
#include        "MoveTo.hh"
#include	"PokemonUtils.hh"
#include        "Script.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		TalkTo : public AAction
  {
  public:
    TalkTo(uint8_t personId);
    ~TalkTo();

  private:
    void	_init();
    void	_update();
    void        _turnToOW();
    bool        _loadScript();
    Script::Instruction     *_searchCmd(std::map<int, Script::Instruction *> &map, uint32_t next);
    Script::Instruction     *_getCurrentCmd();

  private:
    uint8_t     _pid;
    uint8_t     _dir;
    Script      *_script;
  };
};


#endif
