#ifndef		__BATTLEMENU_HH__
#define		__BATTLEMENU_HH__

#include	"PokemonUtils.hh"

class		BattleMenu
{
private:
  struct	Internal
  {
    uint8_t	submenu;
    uint8_t	state;
    uint16_t	unknown;
    uint8_t	battle;
  };

public:
  BattleMenu()
    : _data((Internal *) gbaMem(0x02023e82)),
      _attack((uint32_t *) gbaMem(0x02023FFC)),
      _cursor((uint32_t *) gbaMem(0x02023FF8))
  {}

public:
  uint16_t	isOpen() const { return (_data->battle == 1); }
  uint16_t	getMenu() const { return (_data->submenu == 1 ? 0 : (*_cursor) + 1); }
  uint32_t	getAttack() const { return (*_attack); }
  uint32_t	getCursor() const { return (*_cursor); }

private:
  Internal	*_data;
  uint32_t	*_attack;
  uint32_t	*_cursor;
};

#endif
