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
