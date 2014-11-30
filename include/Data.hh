#ifndef		__DATA_HH__
#define		__DATA_HH__

#include	<vector>
#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"
#include	"World.hh"
#include	"Team.hpp"
#include	"BattleGroup.hpp"
#include	"Player.hh"
#include	"Move.hh"
#include	"Species.hh"
#include	"BagMenu.hh"
#include	"BattleMenu.hh"
#include	"OverWorld.hh"
#include        "MultiChoice.hh"
#include	"IPokeData.hh"

struct		Range
{
  Range(float a = 0, float b = 0) : min(a), max(b) {}

  int		min;
  int		max;
};

class		Data
{
public:
  Data();
  ~Data();

public:
  void		update();

public:
  const std::vector<char *>	&speciesNames() const { return (_speciesNames); }
  char		*speciesName(uint16_t i) const { return (i < _speciesNames.size() ? _speciesNames[i] : NULL); }

  const std::vector<Move>	&moves() const { return (_moves); }
  const Move	&move(uint8_t i) const { return (_moves[(i < _moves.size()) * i]); }

  const std::vector<char *>	&types() const { return (_types); }
  const char	*type(uint8_t i) const { return (_types[(i < _types.size()) * i]); }
  bool		isSpecial(uint8_t type) const { return (type > 9); };

  const std::vector<Species>	&species() const { return (_species); }
  const Species	&species(uint16_t i) const { return (_species[(i < _species.size()) * i]); }

  const std::vector<char *>	&abilityNames() const { return (_abilityNames); }
  char		*abilityName(uint8_t i) const { return (i < _abilityNames.size() ? _abilityNames[i] : NULL); }

  const OverWorld	*overWorlds() const { return (_ows); }
  const OverWorld	&overWorld(uint8_t i) const { return (_ows[(i < 16) * i]); }
  const MultiChoice	*multiChoices() const { return (_mchoice); }
  const MultiChoice	&multiChoice(uint8_t i) const { return (_mchoice[(i <= 0x40) * i]); }

  float		typeEffectiveness(uint8_t atk, uint8_t def) const { return (_typeChart[atk][def]); }
  float		typeEffectiveness(const Move &m, const Species &s) const;
  float		sameTypeAttackBonus(const Move &m, const Species &s) const;
  Range		potentialDamage(const IPokeData &attacker, const IPokeData &target, const Move &m) const;
  float		chanceToHit(const IPokeData &attacker, const IPokeData &target, const Move &m) const;

  Team		&playerTeam() { return (_pteam); }
  Team		&enemyTeam() { return (_eteam); }
  BattleGroup	&battlers() { return (_battlers); }
  World		&world() { return (_world); }
  Player	&player() { return (_player); }
  BagMenu	&bagMenu() { return (_bagMenu); }
  BattleMenu	&battleMenu() { return (_battleMenu); }
  bool		inBattle() { return (*((uint32_t *) gbaMem(0x030030F0)) == 0x80123e5); }

private:
  void		_loadStrings(std::vector<char *> &dest, uint32_t addr, uint8_t len, const char* delim, uint8_t delimsz);
  void		_loadMoves();
  void		_loadSpecies();
  void		_loadTypeEffectiveness();

private:
  // Static
  std::vector<char*>	_speciesNames;
  std::vector<char*>	_moveNames;
  std::vector<char*>	_abilityNames;
  std::vector<char*>	_types;
  std::vector<Move>	_moves;
  std::vector<Species>	_species;
  float			_typeChart[18][18];
  World			_world;
  // Dynamic
  BagMenu		_bagMenu;
  BattleMenu		_battleMenu;
  OverWorld		*_ows;
  MultiChoice           *_mchoice;
  // Manual
  Player	_player;
  Team		_pteam;
  Team		_eteam;
  BattleGroup	_battlers;

public:
  static const Species  &getSpeciesWrapper(uint16_t i) { return (data->species(i)); }

  static Data   *data;
};

#endif
