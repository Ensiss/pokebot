#ifndef		__OVERWORLD_HH__
#define		__OVERWORLD_HH__

#include	"PokemonUtils.hh"

class		OverWorld
{
public:
  OverWorld() {}

public:
  uint16_t	getDestX() const { return (_destX - 7); } // -7 to sync it with world and camera
  uint16_t	getDestY() const { return (_destY - 7); }
  uint16_t	getCurrX() const { return (_currX - 7); }
  uint16_t	getCurrY() const { return (_currY - 7); }
  uint16_t	getX() const { return (_currX - 7); }
  uint16_t	getY() const { return (_currY - 7); }
  uint16_t	getDir() const { return (_dir); }
  uint8_t	getFlags() const { return (_flags); }
  uint8_t	getEventNb() const { return (_evtNb); }
  uint8_t	getMap() const { return (_map); }
  uint8_t	getBank() const { return (_bank); }
  uint16_t	getSpawnX() const { return (_spawnX); }
  uint16_t	getSpawnY() const { return (_spawnY); }

private:
  uint8_t	_temp;		// Temporary variable ?
  uint8_t	_flags;		// 0x01 = locked (in menu or talking)
				// 0x10 = immovable (pokeball in Oak's lab, cuttable tree, etc.)
				// 0x40 = off-screen
  uint16_t	_unknown;

  uint8_t	_unknown2;
  uint8_t	_pictureNb;
  uint16_t	_mvtType;

  uint8_t	_evtNb;
  uint8_t	_map;		// Updated by warps for the player
  uint8_t	_bank;		// Updated by warps for the player
  uint8_t	_jump;		// Unknown. Set to 0x30 when jumping, 0x33 otherwise

  uint16_t	_spawnX;
  uint16_t	_spawnY;

  uint16_t	_destX;
  uint16_t	_destY;
  uint16_t	_currX;
  uint16_t	_currY;
  uint16_t	_dir2;		// _dir * 11
  uint16_t	_unknown5;
  uint32_t	_anim;		// current OW animation ?
  uint16_t	_dir;		// 1 = down, 2 = up, 3 = left, 4 = right
  uint16_t	_unknown6;
};

#endif
