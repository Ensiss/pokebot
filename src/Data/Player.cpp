#include	"Player.hh"
#include	<cstdio>

void		Player::update()
{
  uint32_t	offset, doffset;

  if (!(offset = *((uint32_t *) (internalRAM + 0x05008))) ||
      !(doffset = *((uint32_t *) (internalRAM + 0x0500C))))
    {
      _valid = false;
      return;
    }
  _valid = true;
  _loc = (MapLoc *) (workRAM + offset - WRAM_OFFSET);
  _data = (Internal *) (workRAM + doffset - WRAM_OFFSET);
  for (int i = 0; i < 9; i++)
    _name[i] = (i < 8 && _data->name[i] != 0xFF) * pokeCharsetToAscii(_data->name[i]);
}
