#ifndef		__BATTLEDATA_HH__
#define		__BATTLEDATA_HH__

#include	"PokemonUtils.hh"
#include	"IPokeData.hh"
#include	<cstdio>

class		BattleData : public IPokeData
{
private:
  struct	Internal {
    uint16_t	species;
    uint16_t	atk;
    uint16_t	def;
    uint16_t	speed;
    uint16_t	spatk;
    uint16_t	spdef;
    uint16_t	moves[4];
    uint32_t	ivs;
    uint8_t	hpBuff;
    uint8_t	atkBuff;
    uint8_t	defBuff;
    uint8_t	speedBuff;
    uint8_t	spatkBuff;
    uint8_t	spdefBuff;
    uint8_t	accuracyBuff;
    uint8_t	evasionBuff;
    uint8_t	ability;
    uint8_t	type1;
    uint8_t	type2;
    uint8_t	padding;
    uint8_t	PPs[4];
    uint16_t	currHP;
    uint8_t	level;
    uint8_t	happiness;
    uint16_t	maxHP;
    uint16_t	item;
    uint8_t	nick[11];
    uint8_t	unknown;
    uint8_t	otName[8];
    uint32_t	padding2;
    uint32_t	pid;
    uint32_t	status;
    uint32_t	status2;
    uint32_t	otId;
  };

public:
  BattleData() {}
  BattleData(uint32_t addr)
    : _data((Internal *) gbaMem(addr)) {}
  ~BattleData() {}

private:
  float		_getMultiplier(float n, float d);

public:
  void		update();

public:
  int		getStatus(Status s) const;
  int		isSleeping() const { return (getStatus(ST_SLEEP)); }
  bool		isPoisoned() const { return (getStatus(ST_POISON)); }
  bool		isBurnt() const { return (getStatus(ST_BURN)); }
  bool		isFreezed() const { return (getStatus(ST_FREEZE)); }
  bool		isParalysed() const { return (getStatus(ST_PARALYSIS)); }
  bool		isBadlyPoisoned() const { return (getStatus(ST_BAD_POISON)); }
  uint8_t	getLevel() const { return (_data->level); }

  uint16_t	getHP() const { return (_data->currHP); }
  uint16_t	getMaxHP() const { return (_data->maxHP); }
  uint16_t	getAtk() const { return (_data->atk); }
  uint16_t	getDef() const { return (_data->def); }
  uint16_t	getSpeed() const { return (_data->speed); }
  uint16_t	getSpAtk() const { return (_data->spatk); }
  uint16_t	getSpDef() const { return (_data->spdef); }

  uint16_t	getHPBuff() const { return (_data->hpBuff); }
  uint16_t	getAtkBuff() const { return (_data->atkBuff); }
  uint16_t	getDefBuff() const { return (_data->defBuff); }
  uint16_t	getSpeedBuff() const { return (_data->speedBuff); }
  uint16_t	getSpAtkBuff() const { return (_data->spatkBuff); }
  uint16_t	getSpDefBuff() const { return (_data->spdefBuff); }
  uint16_t	getAccuracyBuff() const { return (_data->accuracyBuff); }
  uint16_t	getEvasionBuff() const { return (_data->evasionBuff); }

  uint16_t	*getMoves() const { return (_data->moves); }
  uint8_t	*getPPs() const { return (_data->PPs); }
  uint16_t	getMove(uint8_t i) const { return (getMoves()[i]); }
  uint8_t	getPP(uint8_t i) const { return (getPPs()[i]); }

private:
  Internal	*_data;
};

#endif
