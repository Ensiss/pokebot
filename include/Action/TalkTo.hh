#ifndef         __ACTION_TALKTO_HH__
#define         __ACTION_TALKTO_HH__

#include	"Data.hh"
#include	"AAction.hh"
#include        "PressButton.hh"
#include        "Wait.hh"
#include        "MoveTo.hh"
#include	"PokemonUtils.hh"
#include        "Script.hh"
#include        "VM.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

namespace	Action
{
  class		TalkTo : public AAction
  {
  public:
    static TalkTo       *create(uint8_t id) { return (new TalkTo(id)); }

  public:
    TalkTo(uint8_t personId, ChoicePts *choices = NULL);
    ~TalkTo();

  private:
    void	_init();
    void	_update();
    void        _turnToOW();
    void        _handleMultiChoice();
    void        _handleYesNo();

    Script::Instruction     *_searchCmd(std::map<int, Script::Instruction *> &map, uint32_t next);
    Script::Instruction     *_getCurrentCmd();

  private:
    uint8_t     _pid;
    uint8_t     _dir;
    uint32_t    _oldNext;
    Script      *_script;
    ChoicePts   *_choices;
    uint8_t     _choiceId;
  };
};


#endif
