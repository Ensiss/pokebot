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
  if ((ret = charsetSubset(0x2D, "&+", c)))
    return (ret);
  if ((ret = charsetSubset(0x5B, "%()", c)))
    return (ret);
  if ((ret = charsetSubset(0xAB, "!?.-", c)))
    return (ret);
  if ((ret = charsetSubset(0xB0, ".\"\"''MF ,x/", c)))
    return (ret);
  return ('?');
}

bool            getFlag(uint16_t flag)
{
  uint32_t	offset;
  uint8_t	byte;

  offset = *((uint32_t *) gbaMem(0x03005008));
  byte = ((uint8_t *) gbaMem(offset + 0xEE0))[flag >> 3];
  return (byte & (1 << (flag % 8)));
}

uint16_t        getVar(uint16_t var)
{
  uint32_t	offset;

  offset = *((uint32_t *) gbaMem(0x03005008));
  return (((uint16_t *) gbaMem(offset + 0x1000))[var - 0x4000]);
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
