#ifndef		__PLAYER_HH__
#define		__PLAYER_HH__

#include	"../vbam/gba/Globals.h"
#include	"PokemonUtils.hh"

class		Player
{
private:
  struct	MapLoc
  {
    uint16_t	x;
    uint16_t	y;
    uint8_t	bankId;
    uint8_t	mapId;
    uint8_t     unknown; // 0xFF when valid ?
  };

  struct	Internal
  {
    uint8_t	name[8];
    uint8_t	gender;
    uint8_t	unknown;
    uint16_t	trainerID;
  };

public:
  Player() {}
  ~Player() {}

public:
  void		update();

public:
  const char	*getName() const { return (_name); }
  uint8_t	getGender() const { return (_data->gender); }
  uint16_t	getTrainerID() const { return (_data->trainerID); }
  uint16_t	getX() const { return (_loc->x); }
  uint16_t	getY() const { return (_loc->y); }
  uint8_t	getBankId() const { return (_loc->bankId); }
  uint8_t	getMapId() const { return (_loc->mapId); }
  bool		isValid() const { return (_valid); }

private:
  Internal	*_data;
  MapLoc	*_loc;
  char		_name[9];
  bool		_valid;
};

#endif
