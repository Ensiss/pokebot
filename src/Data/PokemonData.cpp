#include	"PokemonData.hh"

void		PokemonData::update()
{
  // Convert nickname
  for (int i = 0; i < 11; i++)
    _nick[i] = (i < 10 && _data->nick[i] != 0xFF) * pokeCharsetToAscii(_data->nick[i]);
  // Convert otName
  for (int i = 0; i < 8; i++)
    _otName[i] = (i < 7 && _data->otName[i] != 0xFF) * pokeCharsetToAscii(_data->otName[i]);
  _decryptSubstructures();
}

void		PokemonData::_decryptSubstructures()
{
  uint8_t	max = 24;
  uint8_t	n = 4;
  uint8_t	pick = (1 << n) - 1;
  uint32_t	key = _data->personality ^ _data->otID;
  uint32_t	order = _data->personality % 24;
  void		**subs[4] = {(void **) &_growth, (void **) &_attacks, (void **) &_evs, (void **) &_misc};
  int		i, j;
  int		count;

  for (i = 0; i < 12; i++)
    _xored[i] = *((uint32_t *) (_data->data + (i * 4))) ^ key;
  while (n)
    {
      i = order / (max /= n);
      order %= max;
      for (count = -1, j = 0; j < 4; j++)
	if (pick & (1 << j) && ++count == i)
	  break;
      pick &= ~(1 << j);
      *(subs[j]) = (void *) (_xored + 3 * (4 - n));
      n--;
    }
}

int		PokemonData::getStatus(Status s) const
{
  if (s == ST_SLEEP)
    return (_data->status & 7);
  return (!!(_data->status & (1 << (s + 2))));
}
