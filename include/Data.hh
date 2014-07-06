#ifndef		__DATA_HH__
#define		__DATA_HH__

#include	<vector>
#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"
#include	"World.hh"
#include	"Team.hpp"
#include	"Player.hh"

class		Data
{
public:
  Data();
  ~Data();

public:
  void		update();

public:
  const std::vector<char *>	&names() const { return (_names); }
  char		*name(uint8_t i) const { return (i < _names.size() ? _names[i] : NULL); }
  Team		&playerTeam() { return (_pteam); }
  Team		&enemyTeam() { return (_eteam); }
  World		&world() { return (_world); }
  Player	&player() { return (_player); }

private:
  void		_loadNames();

private:
  // Static
  std::vector<char*>	_names;
  World			_world;
  // Dynamic*
  Player	_player;
  Team		_pteam;
  Team		_eteam;
};

#endif
