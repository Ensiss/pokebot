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
  const std::vector<char *>	&moveNames() const { return (_moveNames); }
  char		*moveName(uint8_t i) const { return (i < _moveNames.size() ? _moveNames[i] : NULL); }
  Team		&playerTeam() { return (_pteam); }
  Team		&enemyTeam() { return (_eteam); }
  World		&world() { return (_world); }
  Player	&player() { return (_player); }

private:
  void		_loadNames();
  void		_loadMoveNames();

private:
  // Static
  std::vector<char*>	_names;
  std::vector<char*>	_moveNames;
  World			_world;
  // Dynamic*
  Player	_player;
  Team		_pteam;
  Team		_eteam;
};

#endif
