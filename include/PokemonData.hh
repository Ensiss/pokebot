#ifndef		__POKEMONDATA_HH__
#define		__POKEMONDATA_HH__

#include	"PokemonUtils.hh"
#include	"IPokeData.hh"

class		PokemonData : public IPokeData
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

public:
  PokemonData() {}
  PokemonData(uint32_t addr)
    : _data((Internal *) gbaMem(addr)) {}
  ~PokemonData() {}

public:
  void		update();

public:
  int		getStatus(Status s) const;
  uint8_t	hasPokerus() const { return (_data->pokerus); }
  uint8_t	getLevel() const { return (_data->level); }

  uint16_t	getHP() const { return (_data->currHP); }
  uint16_t	getMaxHP() const { return (_data->maxHP); }
  uint16_t	getAtk() const { return (_data->atk); }
  uint16_t	getDef() const { return (_data->def); }
  uint16_t	getSpeed() const { return (_data->speed); }
  uint16_t	getSpAtk() const { return (_data->spatk); }
  uint16_t	getSpDef() const { return (_data->spdef); }

  int		getHPBuff() const { return (0); }
  int		getAtkBuff() const { return (0); }
  int		getDefBuff() const { return (0); }
  int		getSpeedBuff() const { return (0); }
  int		getSpAtkBuff() const { return (0); }
  int		getSpDefBuff() const { return (0); }
  int		getAccuracyBuff() const { return (0); }
  int		getEvasionBuff() const { return (0); }

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

private:
  void		_decryptSubstructures();

private:
  Internal	*_data;
  uint32_t	_xored[12];
  Growth	*_growth;
  Attacks	*_attacks;
  EVs		*_evs;
  Misc		*_misc;
};

#endif
