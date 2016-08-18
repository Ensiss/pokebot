#ifndef		__BAGMENU_HH__
#define		__BAGMENU_HH__

#include	"PokemonUtils.hh"

class		BagMenu
{
private:
  struct	Internal
  {
    uint8_t	unknown;
    uint8_t	open;
    uint16_t	pocket;
    uint16_t	cursors[3];
    uint16_t  scrolls[3];
  };

public:
  BagMenu()
    : _data((Internal *) gbaMem(0x0203AD00)), _cursor((uint16_t *) gbaMem(0x030050d8))
  {}

public:
  bool      isOpen() const { return !!_data->open; }
  uint16_t	getPocket() const { return _data->pocket; }
  uint16_t	getCursor() const { return _cursor[1]; }
  uint16_t	getScroll() const { return _cursor[0]; }

private:
  // General bag data
  Internal	*_data;
  // Universal cursor and scroll pocket values that work for all five pockets
  uint16_t  *_cursor;
};

#endif
