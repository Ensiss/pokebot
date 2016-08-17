#ifndef         __ITEM_HH__
#define         __ITEM_HH__

class           Item
{
private:
  struct        Internal
  {
    uint8_t     name[14];
    uint16_t    index;
    uint16_t    price;
    uint8_t     holdEffect;
    uint8_t     parameter;
    uint32_t    descriptionPtr;
    uint16_t    mysteryValue;
    uint8_t     pocket;
    uint8_t     type;
    uint32_t    fieldUsageCodePtr;
    uint32_t    battleUsage;
    uint32_t    battleUsageCodePtr;
    uint32_t    extraParameter;
  };

public:
  Item() {}
  Item(uint32_t addr)
    : _data((Internal *) gbaMem(addr))
  {
    int len = sizeof(_data->name);
    for (int i = 0; i < len; i++)
      _name[i] = (i < len - 1 && _data->name[i] != 0xFF) * pokeCharsetToAscii(_data->name[i]);
  }

public:
  const char    *getName() const { return _name; }
  uint16_t      getIndex() const { return _data->index; }
  uint16_t      getPrice() const { return _data->price; }
  uint8_t       getHoldEffect() const { return _data->holdEffect; }
  uint8_t       getParameter() const { return _data->parameter; }
  uint16_t      getMysteryValue() const { return _data->mysteryValue; }
  uint8_t       getPocket() const { return _data->pocket - 1; }
  uint8_t       getType() const { return _data->type; }
  uint32_t      getFieldUsageCodePtr() const { return _data->fieldUsageCodePtr; }
  uint32_t      getBattleUsage() const { return _data->battleUsage; }
  uint32_t      getBattleUsageCodePtr() const { return _data->battleUsageCodePtr; }
  uint32_t      getExtraParameter() const { return _data->extraParameter; }

private:
  Internal      *_data;
  char          _name[14];
};

#endif
