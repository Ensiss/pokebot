#ifndef		__POKEMONDATA_HH__
#define		__POKEMONDATA_HH__

#include	"PokemonUtils.hh"

class		PokemonData
{
private:
  struct	Internal {
    uint32_t	personality;
    uint32_t	otID;
    uint8_t	nick[10];
    uint16_t	lang;
    uint8_t	otName[7];
    uint8_t	markings;
    uint16_t	checksum;
    uint16_t	padding;
    uint8_t	data[48];
    uint32_t	status;
    uint8_t	level;
    uint8_t	pokerus;
    uint16_t	currHP;
    uint16_t	maxHP;
    uint16_t	atk;
    uint16_t	def;
    uint16_t	speed;
    uint16_t	spatk;
    uint16_t	spdef;
  };

  struct	Growth {
    uint16_t	species;
    uint16_t	item;
    uint32_t	xp;
    uint8_t	ppUp;
    uint8_t	frienship;
    uint16_t	unknown;
  };

  struct	Attacks {
    uint16_t	move1;
    uint16_t	move2;
    uint16_t	move3;
    uint16_t	move4;
    uint8_t	pp1;
    uint8_t	pp2;
    uint8_t	pp3;
    uint8_t	pp4;
  };

  struct	EVs {
    uint8_t	hp;
    uint8_t	atk;
    uint8_t	def;
    uint8_t	speed;
    uint8_t	spatk;
    uint8_t	spdef;
    uint8_t	coolness;
    uint8_t	beauty;
    uint8_t	cuteness;
    uint8_t	smartness;
    uint8_t	toughness;
    uint8_t	feel;
  };

  struct	Misc {
    uint8_t	pokerus;
    uint8_t	metLocation;
    uint16_t	originsInfo;
    uint32_t	ivEggAbility;
    uint32_t	ribbons;
  };

  enum		Status {
    ST_SLEEP,
    ST_POISON,
    ST_BURN,
    ST_FREEZE,
    ST_PARALYSIS,
    ST_BAD_POISON
  };

public:
  PokemonData() {}
  PokemonData(void *addr)
    : _data((Internal *) addr) {}

public:
  void		update();

public:
  const char	*getNick() const { return (_nick); }
  const char	*getOtName() const { return (_otName); }
  int		getStatus(Status s) const;
  int		isSleeping() const { return (getStatus(ST_SLEEP)); }
  bool		isPoisoned() const { return (getStatus(ST_POISON)); }
  bool		isBurnt() const { return (getStatus(ST_BURN)); }
  bool		isFreezed() const { return (getStatus(ST_FREEZE)); }
  bool		isParalysed() const { return (getStatus(ST_PARALYSIS)); }
  bool		isBadlyPoisoned() const { return (getStatus(ST_BAD_POISON)); }
  uint8_t	hasPokerus() const { return (_data->pokerus); }
  uint8_t	getLevel() const { return (_data->level); }
  uint16_t	getHP() const { return (_data->currHP); }
  uint16_t	getMaxHP() const { return (_data->maxHP); }
  uint16_t	getAtk() const { return (_data->atk); }
  uint16_t	getDef() const { return (_data->def); }
  uint16_t	getSpeed() const { return (_data->speed); }
  uint16_t	getSpAtk() const { return (_data->spatk); }
  uint16_t	getSpDef() const { return (_data->spdef); }

  uint16_t	getSpecies() const { return (_growth->species); }
  uint16_t	getItem() const { return (_growth->item); }
  uint32_t	getXP() const { return (_growth->xp); }

  uint8_t	getHPEV() const { return (_evs->hp); }
  uint8_t	getAtkEV() const { return (_evs->atk); }
  uint8_t	getDefEV() const { return (_evs->def); }
  uint8_t	getSpeedEV() const { return (_evs->speed); }
  uint8_t	getSpAtkEV() const { return (_evs->spatk); }
  uint8_t	getSpDefEV() const { return (_evs->spdef); }

  uint16_t	*getMoves() const { return ((uint16_t *) &(_attacks->move1)); }
  uint8_t	*getPPs() const { return ((uint8_t *) &(_attacks->pp1)); }
  uint16_t	getMove(uint8_t i) const { return (getMoves()[i]); }
  uint8_t	getPP(uint8_t i) const { return (getPPs()[i]); }

private:
  void		_decryptSubstructures();

private:
  Internal	*_data;
  char		_nick[11];
  char		_otName[8];
  uint32_t	_xored[12];
  Growth	*_growth;
  Attacks	*_attacks;
  EVs		*_evs;
  Misc		*_misc;
};

#endif
