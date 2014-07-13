#ifndef		__SPECIES_HH__
#define		__SPECIES_HH__

#include	"PokemonUtils.hh"

class		Species
{
private:
  struct	Internal
  {
    uint8_t	hp;
    uint8_t	atk;
    uint8_t	def;
    uint8_t	speed;
    uint8_t	spatk;
    uint8_t	spdef;
    uint8_t	type1;
    uint8_t	type2;
    uint8_t	catchRate;
    uint8_t	baseExpYeld;
    uint16_t	effortYield;
    uint16_t	item1;
    uint16_t	item2;
    uint8_t	gender;
    uint8_t	eggCycles;
    uint8_t	friendship;
    uint8_t	levelUpType;
    uint8_t	eggGroup1;
    uint8_t	eggGroup2;
    uint8_t	ability1;
    uint8_t	ability2;
    uint8_t	safariZoneRate;
    uint8_t	colorFlip;
    uint16_t	padding;
  };

public:
  Species() {}
  Species(uint32_t addr, char *name)
    : _data((Internal *) gbaMem(addr)), _name(name)
  {}

public:
  uint8_t	getHP() const { return (_data->hp); }
  uint16_t	getAtk() const { return (_data->atk); }
  uint16_t	getDef() const { return (_data->def); }
  uint16_t	getSpeed() const { return (_data->speed); }
  uint16_t	getSpAtk() const { return (_data->spatk); }
  uint16_t	getSpDef() const { return (_data->spdef); }
  uint8_t	*getTypes() const { return ((uint8_t *) &(_data->type1)); }
  uint8_t	getType(uint8_t t) const { return (getTypes()[(t < 2) * t]); }
  uint8_t	*getAbilities() const { return ((uint8_t *) &(_data->ability1)); }
  uint8_t	getAbility(uint8_t t) const { return (getAbilities()[(t < 2) * t]); }
  uint16_t	*getItems() const { return ((uint16_t *) &(_data->item1)); }
  uint16_t	getItem(uint8_t t) const { return (getItems()[(t < 2) * t]); }
  const char	*getName() const { return (_name); }

private:
  Internal	*_data;
  char		*_name;
};

#endif
