#ifndef		__DATA_HH__
#define		__DATA_HH__

#include	<vector>
#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"
#include	"World.hh"
#include	"Team.hpp"
#include	"Player.hh"
#include	"Move.hh"
#include	"Species.hh"

class		Data
{
public:
  Data();
  ~Data();

public:
  void		update();

public:
  const std::vector<char *>	&speciesNames() const { return (_speciesNames); }
  char		*speciesName(uint8_t i) const { return (i < _speciesNames.size() ? _speciesNames[i] : NULL); }

  const std::vector<Move>	&moves() const { return (_moves); }
  const Move	&move(uint8_t i) const { return (_moves[(i < _moves.size()) * i]); }

  const std::vector<char *>	&types() const { return (_types); }
  const char	*type(uint8_t i) const { return (_types[(i < _types.size()) * i]); }

  const std::vector<Species>	&species() const { return (_species); }
  const Species	&species(uint8_t i) const { return (_species[(i < _species.size()) * i]); }

  const std::vector<char *>	&abilityNames() const { return (_abilityNames); }
  char		*abilityName(uint8_t i) const { return (i < _abilityNames.size() ? _abilityNames[i] : NULL); }

  Team		&playerTeam() { return (_pteam); }
  Team		&enemyTeam() { return (_eteam); }
  World		&world() { return (_world); }
  Player	&player() { return (_player); }

private:
  void		_loadStrings(std::vector<char *> &dest, uint32_t addr, uint8_t len, const char* delim, uint8_t delimsz);
  void		_loadMoves();
  void		_loadSpecies();

private:
  // Static
  std::vector<char*>	_speciesNames;
  std::vector<char*>	_moveNames;
  std::vector<char*>	_abilityNames;
  std::vector<char*>	_types;
  std::vector<Move>	_moves;
  std::vector<Species>	_species;
  World			_world;
  // Dynamic*
  Player	_player;
  Team		_pteam;
  Team		_eteam;
};

#endif
