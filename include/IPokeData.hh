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

public:
  virtual		~IPokeData() {}
  virtual void		update() = 0;

public:
  const char		*getNick() const { return (_nick); }
  const char		*getOtName() const { return (_otName); }
  virtual int		getStatus(Status s) const = 0;
  virtual int		isSleeping() const = 0;
  virtual bool		isPoisoned() const = 0;
  virtual bool		isBurnt() const = 0;
  virtual bool		isFreezed() const = 0;
  virtual bool		isParalysed() const = 0;
  virtual bool		isBadlyPoisoned() const = 0;
  virtual uint8_t	getLevel() const = 0;
  virtual uint16_t	getHP() const = 0;
  virtual uint16_t	getMaxHP() const = 0;
  virtual uint16_t	getAtk() const = 0;
  virtual uint16_t	getDef() const = 0;
  virtual uint16_t	getSpeed() const = 0;
  virtual uint16_t	getSpAtk() const = 0;
  virtual uint16_t	getSpDef() const = 0;

  virtual uint16_t	*getMoves() const = 0;
  virtual uint8_t	*getPPs() const = 0;
  virtual uint16_t	getMove(uint8_t i) const = 0;
  virtual uint8_t	getPP(uint8_t i) const = 0;

protected:
  char		_nick[12];
  char		_otName[9];
};

#endif
