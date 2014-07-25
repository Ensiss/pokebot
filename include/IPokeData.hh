#ifndef		__IPOKEDATA_HH__
#define		__IPOKEDATA_HH__

class		IPokeData
{
protected:
  enum		Status {
    ST_SLEEP,
    ST_POISON,
    ST_BURN,
    ST_FREEZE,
    ST_PARALYSIS,
    ST_BAD_POISON
  };

private:
  float		_getMultiplier(float n, float d) const
  {
    float		r = ((n < 0 ? -n : n) + d) / d;
    return (n < 0 ? 1 / r : r);
  }

public:
  virtual		~IPokeData() {}
  virtual void		update() = 0;

public:
  const char		*getNick() const { return (_nick); }
  const char		*getOtName() const { return (_otName); }
  uint16_t		getRealAtk() const { return ((float) getAtk() * _getMultiplier(getAtkBuff(), 2)); }
  uint16_t		getRealDef() const { return ((float) getDef() * _getMultiplier(getDefBuff(), 2)); }
  uint16_t		getRealSpeed() const { return ((float) getSpeed() * _getMultiplier(getSpeedBuff(), 2)); }
  uint16_t		getRealSpAtk() const { return ((float) getSpAtk() * _getMultiplier(getSpAtkBuff(), 2)); }
  uint16_t		getRealSpDef() const { return ((float) getSpDef() * _getMultiplier(getSpDefBuff(), 2)); }
  uint16_t		getRealAccuracy() const { return (100 * _getMultiplier(getAccuracyBuff(), 3)); }
  uint16_t		getRealEvasion() const { return (100 * _getMultiplier(getEvasionBuff(), 3)); }

  int			isSleeping() const { return (getStatus(ST_SLEEP)); }
  bool			isPoisoned() const { return (getStatus(ST_POISON)); }
  bool			isBurnt() const { return (getStatus(ST_BURN)); }
  bool			isFreezed() const { return (getStatus(ST_FREEZE)); }
  bool			isParalysed() const { return (getStatus(ST_PARALYSIS)); }
  bool			isBadlyPoisoned() const { return (getStatus(ST_BAD_POISON)); }

  uint16_t		getMove(uint8_t i) const { return (getMoves()[i]); }
  uint8_t		getPP(uint8_t i) const { return (getPPs()[i]); }

public:
  virtual int		getStatus(Status s) const = 0;
  virtual uint8_t	getLevel() const = 0;
  virtual uint16_t	getSpecies() const = 0;

  virtual uint16_t	getHP() const = 0;
  virtual uint16_t	getMaxHP() const = 0;
  virtual uint16_t	getAtk() const = 0;
  virtual uint16_t	getDef() const = 0;
  virtual uint16_t	getSpeed() const = 0;
  virtual uint16_t	getSpAtk() const = 0;
  virtual uint16_t	getSpDef() const = 0;

  virtual int		getAtkBuff() const = 0;
  virtual int		getDefBuff() const = 0;
  virtual int		getSpeedBuff() const = 0;
  virtual int		getSpAtkBuff() const = 0;
  virtual int		getSpDefBuff() const = 0;
  virtual int		getAccuracyBuff() const = 0;
  virtual int		getEvasionBuff() const = 0;

  virtual uint16_t	*getMoves() const = 0;
  virtual uint8_t	*getPPs() const = 0;

protected:
  char		_nick[12];
  char		_otName[9];
};

#endif
