#ifndef		__BOT_HH__
#define		__BOT_HH__

#include	<deque>
#include	"Data.hh"
#include	"Action.hh"
#include	"AAction.hh"
#include	"../vbam/sdl/SDLGlobals.h"

class		Bot
{
public:
  Bot();
  ~Bot();

public:
  void		update();
  void		queue(AAction *action);

public:
  int		getState() const { return (_state); }
  void		setBattleAction(AAction *action) { _battle = action; }

private:
  std::deque<AAction*>	_queue;
  AAction		*_action;
  AAction		*_battle;
  bool			_battleState;
  int			_state;
};

#endif
