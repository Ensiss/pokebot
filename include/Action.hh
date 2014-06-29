#ifndef		__ACTION_HH__
#define		__ACTION_HH__

#include	"Movement.hh"
#include	"Data.hh"

class		Action
{
public:
  Action(Data &d)
    : _movement(d)
  {}
  ~Action() {}

public:
  void		update();
  Movement	&movement() { return (_movement); }
  void		moveTo(uint16_t x, uint16_t y) { _movement.moveTo(x, y); }

private:
  Movement	_movement;
};

#endif
