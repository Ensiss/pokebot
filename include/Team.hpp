#ifndef		__TEAM_HH__
#define		__TEAM_HH__

#include	"PokemonData.hh"

class		Team
{
public:
  Team() {}
  Team(uint32_t addr)
  {
    for (int i = 0; i < 6; i++)
      list[i] = PokemonData(addr + 100 * i);
  }

public:
  const PokemonData	&operator[](uint8_t i)
  {
    if (i >= 6) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Team");
    return list[i];
  }

public:
  void		update()
  {
    for (int i = 0; i < 6; i++)
      list[i].update();
  }

protected:
  PokemonData	list[6];
};

#endif
