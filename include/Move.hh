#ifndef		__MOVE_HH__
#define		__MOVE_HH__

#include	"PokemonUtils.hh"

class		Move
{
private:
  struct	Internal
  {
    uint8_t	effect;
    uint8_t	power;
    uint8_t	type;
    uint8_t	accuracy;
    uint8_t	pp;
    uint8_t	effectAccuracy;
    uint8_t	target;
    uint8_t	priority;
    uint8_t	flags;
    uint8_t	padding[3];
  };

public:
  Move() {}
  Move(uint32_t addr, char *name)
    : _data((Internal *) gbaMem(addr)), _name(name)
  {}

public:
  uint8_t	getPower() const { return (_data->power); }
  uint8_t	getType() const { return (_data->type); }
  uint8_t	getAccuracy() const { return (_data->accuracy); }
  uint8_t	getPP() const { return (_data->pp); }
  uint8_t	getEffectAccuracy() const { return (_data->effectAccuracy); }
  uint8_t	getTarget() const { return (_data->target); }
  uint8_t	getPriority() const { return (_data->priority); }
  uint8_t	getFlags() const { return (_data->flags); }
  const char	*getName() const { return (_name); }

private:
  Internal	*_data;
  char		*_name;
};

#endif
