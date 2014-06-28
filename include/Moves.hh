#ifndef		__MOVES_HH__
#define		__MOVES_HH__

class		Moves
{
  struct	Internal
  {
    uint8_t	effect;
    uint8_t	power;
    uint8_t	type;
    uint8_t	accuracy;
    uint8_t	pp;
    uint8_t	effectAccuracy;
    uint8_t	targets;
    uint8_t	priority;
    uint8_t	flags;
    uint8_t	padding[3];
  };

  
};

#endif
