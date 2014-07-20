#ifndef		__AACTION_HH__
#define		__AACTION_HH__

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
  virtual	~AAction() {}
  virtual void	update() = 0;

public:
  Action::State	getState() const { return (_state); }

protected:
  Action::State	_state;
};

#endif
