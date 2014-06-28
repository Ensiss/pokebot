#include	"Player.hh"

void		Player::update()
{
  uint32_t	offset = *((uint32_t *) (internalRAM + 0x05008)) - WRAM_OFFSET;
  _loc = (MapLoc *) (workRAM + offset);
  offset = *((uint32_t *) (internalRAM + 0x0500C)) - WRAM_OFFSET;
  _data = (Internal *) (workRAM + offset);
  for (int i = 0; i < 9; i++)
    _name[i] = (i < 8 && _data->name[i] != 0xFF) * pokeCharsetToAscii(_data->name[i]);
}
