#include	"BattleData.hh"

void		BattleData::update()
{
  // Convert nickname
  for (int i = 0; i < 12; i++)
    _nick[i] = (i < 11 && _data->nick[i] != 0xFF) * pokeCharsetToAscii(_data->nick[i]);
  // Convert otName
  for (int i = 0; i < 9; i++)
    _otName[i] = (i < 8 && _data->otName[i] != 0xFF) * pokeCharsetToAscii(_data->otName[i]);
}

int		BattleData::getStatus(Status s) const
{
  if (s == ST_SLEEP)
    return (_data->status & 7);
  return (!!(_data->status & (1 << (s + 2))));
}
