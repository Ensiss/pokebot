#ifndef		__AACTION_HH__
#define		__AACTION_HH__

#include	<queue>
#include	"Data.hh"

namespace	Action
{
  enum		State
    {
      NOT_STARTED,
      RUNNING,
      FINISHED,
      ERROR
    };
  extern Data	*data;
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
  void		queue(AAction *act) { _actions.push(act); }
  void		reset() { _state = Action::NOT_STARTED; }
  Action::State	getState() const { return (_state); }

protected:
  Data			&_data;
  Action::State		_state;
  std::queue<AAction *>	_actions;
};

#endif
