#ifndef		__TEAM_HH__
#define		__TEAM_HH__

#include	"PokemonData.hh"

class		Team
{
public:
  Team() {}
  Team(uint64_t addr)
  {
    for (int i = 0; i < 6; i++)
      list[i] = PokemonData((void *) (addr + 100 * i));
  }

public:
  PokemonData	&operator[](uint8_t i)
  {
    return (list[(i >= 0 && i < 6) * i]);
  }

public:
  void		update()
  {
    for (int i = 0; i < 6; i++)
      list[i].update();
  }

public:
  PokemonData	list[6];
};

#endif
