#ifndef		__BATTLEGROUP_HH__
#define		__BATTLEGROUP_HH__

#include	"BattleData.hh"

class		BattleGroup
{
public:
  BattleGroup() {}
  BattleGroup(uint32_t addr)
  {
    for (int i = 0; i < 4; i++)
      list[i] = BattleData(addr + 88 * i);
  }

public:
  const BattleData	&operator[](uint8_t i)
  {
    if (i >= 4) throw std::out_of_range("Index " + std::to_string(i) + " out of bounds for BattleGroup");
    return list[i];
  }

public:
  void		update()
  {
    for (int i = 0; i < 4; i++)
      list[i].update();
  }

protected:
  BattleData	list[4];
};

#endif
