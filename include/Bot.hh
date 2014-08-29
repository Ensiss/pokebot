#ifndef		__BOT_HH__
#define		__BOT_HH__

#include	<deque>
#include	"Data.hh"
#include	"Action/AAction.hh"
#include	"../vbam/sdl/SDLGlobals.h"

class		Bot
{
public:
  Bot();
  ~Bot();

private:
  void		_saveKeyState();
  void		_loadKeyState();

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
  uint16_t		_keyState;
};

#endif
