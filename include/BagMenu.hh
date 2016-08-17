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
    : _data((Internal *) gbaMem(0x0203AD00))
  {}

public:
  bool      isOpen() const { return !!_data->open; }
  uint16_t	getPocket() const { return _data->pocket; }
  uint16_t	getCursor() const {
    if (_data->pocket >= 3) throw std::out_of_range("Index " + std::to_string(_data->pocket) + " out of bounds for BagMenu.Cursor");
    return _data->cursors[_data->pocket];
  }
  uint16_t	getScroll() const {
    if (_data->pocket >= 3) throw std::out_of_range("Index " + std::to_string(_data->pocket) + " out of bounds for BagMenu.Scroll");
    return _data->scrolls[_data->pocket];
  }

private:
  Internal	*_data;
};

#endif
