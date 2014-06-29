#ifndef		__MOVEMENT_HH__
#define		__MOVEMENT_HH__

#include	<stdint.h>
#include	"World.hh"
#include	"Data.hh"
#include	"../vbam/sdl/inputSDL.h"
#include	"../vbam/sdl/SDLGlobals.h"

class		Movement
{
public:
  Movement(Data &data)
    : _data(data), _path(NULL)
  {}
  ~Movement() {}

public:
  void		update();
  void		moveTo(uint16_t x, uint16_t y);
  World::Path	*getPath() const { return (_path); }

private:
  void		_releaseKeys();

private:
  Data		&_data;
  World::Path	*_path;
  uint16_t	_tx;
  uint16_t	_ty;
  uint16_t	_oldx;
  uint16_t	_oldy;
  uint8_t	_pathi;
};

#endif
