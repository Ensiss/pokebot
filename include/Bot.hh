#ifndef		__BOT_HH__
#define		__BOT_HH__

#include	<deque>
#include	"Data.hh"
#include	"Action.hh"
#include	"IAction.hh"

class		Bot
{
public:
  Bot();
  ~Bot();

public:
  void		update();
  void		queue(IAction *action);

private:
  std::deque<IAction*>	_queue;
  IAction		*_action;
};

#endif
