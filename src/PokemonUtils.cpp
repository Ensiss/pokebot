#include	"PokemonUtils.hh"

char		pokeCharsetToAscii(uint8_t c)
{
  if (c >= 0xA1 && c < 0xAB)
    return (c - 0xA1 + '0');
  if (c >= 0xBB && c < 0xD5)
    return (c - 0xBB + 'A');
  if (c >= 0xD5 && c < 0xEF)
    return (c - 0xD5 + 'a');
  return ('?');
}

void		*gbaMem(uint32_t ptr)
{
  uint8_t	loc = ptr >> 24;
  uint8_t	*ret;

  if (loc == 2)
    ret = workRAM;
  else if (loc == 3)
    ret = internalRAM;
  else if (loc == 8)
    ret = rom;
  return (ret + (ptr & 0xFFFFFF));
}
