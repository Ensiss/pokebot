#include	"Data.hh"

Data::Data()
  : _pteam(0x02024284),
    _eteam(0x0202402C)
{
  _loadStrings(_speciesNames, 0x8245EE0, 11, "\xAE\xFF", 2);
  _loadStrings(_moveNames, 0x8247094, 13, "\x00", 1);
  _loadStrings(_abilityNames, 0x824FC4D, 13, "\x00", 1);
  _loadStrings(_types, 0x824F1A0, 7, "\x00", 1);
  _loadMoves();
  _loadSpecies();
  _loadTypeEffectiveness();
}

Data::~Data()
{
  for (int i = 0; i < _speciesNames.size(); i++)
    delete _speciesNames[i];
  for (int i = 0; i < _moveNames.size(); i++)
    delete _moveNames[i];
  for (int i = 0; i < _abilityNames.size(); i++)
    delete _abilityNames[i];
}

void		Data::update()
{
  _player.update();
  _pteam.update();
  _eteam.update();
}

float		Data::typeEffectiveness(const Move &m, const Species &sp) const
{
  float		eff = _typeChart[m.getType()][sp.getType(0)];

  if (sp.getType(0) != sp.getType(1))
    eff *= _typeChart[m.getType()][sp.getType(1)];
  return (eff);
}

void		Data::_loadStrings(std::vector<char *> &dest, uint32_t addr, uint8_t len, const char* delim, uint8_t delimsz)
{
  int		i, id;
  uint8_t	*ptr;

  for (id = 0; id >= 0; id++)
    {
      ptr = (uint8_t *) gbaMem(addr) + id * len;
      dest.push_back(new char[len]());
      for (i = 0; i < delimsz; i++)
	if ((uint8_t) (delim[i]) != ptr[i])
	  break;
      if (i == delimsz)
	return;
      for (i = 0; i < len; i++)
	dest[id][i] = (i < len - 1 && ptr[i] != 0xFF) * pokeCharsetToAscii(ptr[i]);
    }
}

void		Data::_loadMoves()
{
  for (uint16_t i = 0; i < _moveNames.size(); i++)
    _moves.push_back(Move(0x08250C04 + i * 12, _moveNames[i]));
}

void		Data::_loadSpecies()
{
  for (uint16_t i = 0; i < _speciesNames.size(); i++)
    _species.push_back(Species(0x08254784 + i * 28, _speciesNames[i]));
}

void		Data::_loadTypeEffectiveness()
{
  for (int j = 0; j < 18; j++)
    for (int i = 0; i < 18; i++)
      _typeChart[j][i] = 1.0;
  for (uint8_t *ptr = (uint8_t *) gbaMem(0x0824F050); *ptr != 0xFF; ptr += 3)
    {
      if (ptr[0] < 18)
	{
	  float	val = ptr[2] == 0x14 ? 2.0 : ptr[2] == 0x05 ? 0.5 : 0.0;
	  _typeChart[ptr[0]][ptr[1]] = val;
	}
    }
}
