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

private:
  std::deque<AAction*>	_queue;
  AAction		*_action;
};

#endif
