#ifndef		__AACTION_HH__
#define		__AACTION_HH__

class		AAction
{
public:
  virtual	~AAction() {}
  virtual void	update() = 0;

public:
  int		getState() { return (_state); }

public:
  int		_state;
};

#endif
