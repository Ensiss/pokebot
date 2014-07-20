#ifndef		__IACTION_HH__
#define		__IACTION_HH__

class		IAction
{
public:
  virtual	~IAction() {}
  virtual void	update() = 0;
};

#endif
