#include	"PokemonUtils.hh"
#include	<cstdio>

static char	charsetSubset(uint8_t off, const char *set, uint8_t c)
{
  uint8_t	i = -1;

  while (set[++i])
    if (c == off + i)
      return (set[i]);
  return (0);
}

char		pokeCharsetToAscii(uint8_t c)
{
  char		ret;

  if (c >= 0xA1 && c < 0xAB)
    return (c - 0xA1 + '0');
  if (c >= 0xBB && c < 0xD5)
    return (c - 0xBB + 'A');
  if (c >= 0xD5 && c < 0xEF)
    return (c - 0xD5 + 'a');
  if (!c || c > 0xFA)
    return (' ');
  if (c == 0xF0)
    return (':');
  if (ret = charsetSubset(0x2D, "&+", c))
    return (ret);
  if (ret = charsetSubset(0x5B, "%()", c))
    return (ret);
  if (ret = charsetSubset(0xAB, "!?.-", c))
    return (ret);
  if (ret = charsetSubset(0xB0, ".\"\"''MF ,x/", c))
    return (ret);
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
