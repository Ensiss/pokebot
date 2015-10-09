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
    uint16_t	items[3];
  };

public:
  BagMenu()
    : _data((Internal *) gbaMem(0x0203AD00))
  {}

public:
  uint16_t	isOpen() const { return (_data->open); }
  uint16_t	getPocket() const { return (_data->pocket); }
  uint16_t	getItem() const {
    if (_data->pocket >= 3) throw std::out_of_range("Index " + std::to_string(_data->pocket) + " out of bounds for BagMenu.Item");
    return _data->items[_data->pocket];
  }

private:
  Internal	*_data;
};

#endif
