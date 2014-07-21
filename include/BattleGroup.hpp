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
  BattleData	&operator[](uint8_t i)
  {
    return (list[(i >= 0 && i < 4) * i]);
  }

public:
  void		update()
  {
    for (int i = 0; i < 4; i++)
      list[i].update();
  }

public:
  BattleData	list[4];
};

#endif
