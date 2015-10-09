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

public:
  void		update();

public:
  int		getStatus(Status s) const;
  uint8_t	getLevel() const { return (_data->level); }
  uint16_t	getSpeciesId() const { return (_data->species); }

  uint16_t	getHP() const { return (_data->currHP); }
  uint16_t	getMaxHP() const { return (_data->maxHP); }
  uint16_t	getAtk() const { return (_data->atk); }
  uint16_t	getDef() const { return (_data->def); }
  uint16_t	getSpeed() const { return (_data->speed); }
  uint16_t	getSpAtk() const { return (_data->spatk); }
  uint16_t	getSpDef() const { return (_data->spdef); }

  int		getHPBuff() const { return (_data->hpBuff - 6); }
  int		getAtkBuff() const { return (_data->atkBuff - 6); }
  int		getDefBuff() const { return (_data->defBuff - 6); }
  int		getSpeedBuff() const { return (_data->speedBuff - 6); }
  int		getSpAtkBuff() const { return (_data->spatkBuff - 6); }
  int		getSpDefBuff() const { return (_data->spdefBuff - 6); }
  int		getAccuracyBuff() const { return (_data->accuracyBuff - 6); }
  int		getEvasionBuff() const { return (_data->evasionBuff - 6); }

  uint16_t	*getMovesId() const { return (_data->moves); }
  uint8_t	*getPPs() const { return (_data->PPs); }

private:
  Internal	*_data;
};

#endif
