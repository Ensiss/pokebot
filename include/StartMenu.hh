#pragma once

#include  <cstring>
#include	"PokemonUtils.hh"

class		StartMenu
{
private:
  struct	Internal
  {
    uint32_t  activeContext;
    uint8_t   cursor;
    uint8_t   nbItems;
    uint8_t   itemIndices[9];
    uint8_t   state;
  };

public:
  StartMenu()
    : _data((Internal *) gbaMem(0x020370F0)), _dialogBox((uint8_t *) gbaMem(0x020204C0))
  {}

public:
  bool      isOpen() const {
    // Ugly hack: check if the start menu dialog box is open
    static uint8_t ref[12] = {0x00, 0x16, 0x01, 0x07, 0x0d, 0x0f, 0x3d, 0x01, 0x60, 0x2d, 0x00, 0x02};
    return !memcmp(_dialogBox, ref, 12);
  }
  uint8_t   getCursor() const { return _data->cursor; }
  uint8_t   getNbItems() const { return _data->nbItems; }
  uint8_t   getItemIndex(uint8_t id) const {
    if (id >= 9) throw std::out_of_range("Index " + std::to_string(id) + " out of bounds for StartMenu.ItemIndex");
    return _data->itemIndices[id];
  }
  uint8_t   getState() const { return _data->state; }

private:
  Internal	*_data;
  uint8_t   *_dialogBox;
};

