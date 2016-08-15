#pragma once

#include	"PokemonUtils.hh"

enum PocketType
  {
    POCKET_MAIN,
    POCKET_KEY_ITEMS,
    POCKET_BALLS,
    POCKET_HM_CASE,
    POCKET_BERRY_POUCH
  };

class BagItem
{
public:
  BagItem() {}

  uint16_t  getId() const { return _id; }
  uint16_t  getQuantity() const {
    uint32_t key = gbaMem<uint32_t>(gbaMem<uint32_t>(0x0300500C) + 0x0F20);
    return _quantity ^ (key & 0xFFFF);
  }

private:
  uint16_t  _id;
  uint16_t  _quantity;
};

class BagPocket
{
public:
  BagPocket() {}

  const BagItem &getItem(uint32_t id) const {
    if (id > _capacity) throw  std::out_of_range("Index " + std::to_string(id) + " out of bounds for BagPocket.Item");
    return ((BagItem *) gbaMem(_items))[id];
  }
  uint32_t  getCapacity() const { return _capacity; }

private:
  uint32_t  _items;
  uint32_t  _capacity;
};

class Bag
{
public:
  Bag()
    : _pockets((BagPocket *) gbaMem(0x0203988C))
  {}

  const BagPocket &getPocket(uint8_t id) const {
    if (id > 4) throw std::out_of_range("Index " + std::to_string(id) + " out of bounds for Bag.Pocket");
    return _pockets[id];
  }

private:
  BagPocket *_pockets;
};
