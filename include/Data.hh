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
#include	"Item.hh"
#include	"Bag.hh"
#include	"BagMenu.hh"
#include	"BattleMenu.hh"
#include	"OverWorld.hh"
#include  "MultiChoice.hh"
#include	"IPokeData.hh"

struct		Range
{
  Range(float a = 0, float b = 0) : min(a), max(b) {}
  int           getMin() const { return (min); }
  int           getMax() const { return (max); }

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
  const std::vector<char *>	&getSpeciesNames() const { return _speciesNames; }
  char		*getSpeciesName(uint16_t i) const {
    if (i >= _speciesNames.size()) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.SpeciesName");
    return _speciesNames[i];
  }

  const std::vector<char *>	&getTypeNames() const { return _typeNames; }
  const char	*getTypeName(uint8_t i) const {
    if (i >= _typeNames.size()) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.TypeName");
    return _typeNames[i];
  }
  bool		isSpecial(uint8_t type) const { return (type > 9); };

  const std::vector<char *>	&getAbilityNames() const { return _abilityNames; }
  char		*getAbilityName(uint8_t i) const {
    if (i >= _abilityNames.size()) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.AbilityName");
    return _abilityNames[i];
  }

  const std::vector<Move>	&getMoves() const { return _moves; }
  const Move	&getMove(uint8_t i) const {
    if (i >= _moves.size()) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.Moves");
    return _moves[i];
  }

  const std::vector<Species>	&getSpecies() const { return _species; }
  const Species	&getSpecies(uint16_t i) const {
    if (i >= _species.size()) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.Species");
    return _species[i];
  }

  const std::vector<Item>	&getItems() const { return _items; }
  const Item	&getItem(uint16_t i) const {
    if (i >= _items.size()) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.Item");
    return _items[i];
  }

  const OverWorld	*getOverWorlds() const { return _ows; }
  const OverWorld	&getOverWorld(uint8_t i) const {
    if (i >= 16) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.OverWorld");
    return _ows[i];
  }

  const MultiChoice	*getMultiChoices() const { return _mchoice; }
  const MultiChoice	&getMultiChoice(uint8_t i) const {
    if (i > 0x40) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for Data.MultiChoice");
    return _mchoice[i];
  }

  float		typeEffectiveness(uint8_t atk, uint8_t def) const {
    if (atk >= 18) throw std::out_of_range("Index " + std::to_string(atk) + " out of bounds for Data.TypeEffectiveness.Attack");
    if (def >= 18) throw std::out_of_range("Index " + std::to_string(def) + " out of bounds for Data.TypeEffectiveness.Defense");
    return _typeChart[atk][def];
  }
  float		typeEffectiveness(const Move &m, const Species &s) const;
  float		sameTypeAttackBonus(const Move &m, const Species &s) const;
  Range		potentialDamage(const IPokeData &attacker, const IPokeData &target, const Move &m) const;
  float		chanceToHit(const IPokeData &attacker, const IPokeData &target, const Move &m) const;

  Team		&getPlayerTeam() { return (_pteam); }
  Team		&getEnemyTeam() { return (_eteam); }
  BattleGroup	&getBattlers() { return (_battlers); }
  World		&getWorld() { return (_world); }
  Player	&getPlayer() { return (_player); }
  BagMenu	&getBagMenu() { return (_bagMenu); }
  Bag     &getBag() { return (_bag); }
  BattleMenu	&getBattleMenu() { return (_battleMenu); }
  bool		isInBattle() { return (*((uint32_t *) gbaMem(0x030030F0)) == 0x80123e5); }

private:
  void		_loadStrings(std::vector<char *> &dest, uint32_t addr, uint8_t len, const char* delim, uint8_t delimsz);
  void		_loadMoves();
  void		_loadSpecies();
  void		_loadItems();
  void		_loadTypeEffectiveness();

private:
  // Static
  std::vector<char*>	_speciesNames;
  std::vector<char*>	_moveNames;
  std::vector<char*>	_abilityNames;
  std::vector<char*>	_typeNames;
  std::vector<Move>	_moves;
  std::vector<Species>	_species;
  std::vector<Item>     _items;
  float			_typeChart[18][18];
  World			_world;
  // Dynamic
  BagMenu		_bagMenu;
  Bag       _bag;
  BattleMenu		_battleMenu;
  OverWorld		*_ows;
  MultiChoice           *_mchoice;
  // Manual
  Player	_player;
  Team		_pteam;
  Team		_eteam;
  BattleGroup	_battlers;

public:
  static const World::Map       &getMap(uint8_t bank, uint8_t map) { return (data->getWorld().getMap(bank, map)); }
  static const World::Map       &getCurrentMap() { return (data->getWorld().getMap(data->getPlayer().getBankId(), data->getPlayer().getMapId())); }
  static const PokemonData      &getPlayerPokemonWrapper(uint8_t i) { return (data->getPlayerTeam()[i]); }
  static const PokemonData      &getEnemyPokemonWrapper(uint8_t i) { return (data->getEnemyTeam()[i]); }
  static const BattleData       &getBattlerWrapper(uint8_t i) { return (data->getBattlers()[i]); }
  static const OverWorld        &getOverWorldWrapper(uint8_t i) { return (data->getOverWorld(i)); }
  static const BattleMenu       &getBattleMenuWrapper() { return data->_battleMenu; }
  static const BagMenu          &getBagMenuWrapper() { return data->_bagMenu; }
  static const Bag              &getBagWrapper() { return data->_bag; }
  static const Player           &getPlayerWrapper() { return data->_player; }
  static const Species  &getSpeciesWrapper(uint16_t i) { return (data->getSpecies(i)); }
  static const Item     &getItemWrapper(uint16_t i) { return (data->getItem(i)); }
  static const Move     &getMoveWrapper(uint16_t i) { return (data->getMove(i)); }
  static const char     *getTypeNameWrapper(uint8_t i) { return (data->getTypeName(i)); }
  static const char     *getAbilityNameWrapper(uint8_t i) { return (data->getAbilityName(i)); }
  static bool           isInBattleWrapper() { return data->isInBattle(); }

  static Data   *data;
};

#endif
