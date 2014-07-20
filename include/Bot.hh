#ifndef		__BOT_HH__
#define		__BOT_HH__

#include	<deque>
#include	"Data.hh"
#include	"Action.hh"
#include	"AAction.hh"

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

private:
  std::deque<AAction*>	_queue;
  AAction		*_action;
  int			_state;
};

#endif
