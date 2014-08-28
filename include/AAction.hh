#ifndef		__AACTION_HH__
#define		__AACTION_HH__

#include	<queue>

namespace	Action
{
  enum		State
    {
      NOT_STARTED,
      RUNNING,
      FINISHED,
      ERROR
    };
};

class		AAction
{
public:
  AAction();
  Action::State	update();
  virtual	~AAction() {}

private:
  virtual void	_update() = 0;
  virtual void	_init() = 0;

public:
  Action::State	getState() const { return (_state); }

protected:
  Action::State		_state;
  std::queue<AAction *>	_actions;
};

#endif
