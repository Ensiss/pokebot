#include	"Data.hh"

Data::Data()
  : _pteam((uint64_t) (workRAM + PTEAM_PTR)),
    _eteam((uint64_t) (workRAM + ETEAM_PTR))
{
  _loadStrings(_speciesNames, 0x8245EE0, 11, "\xAE\xFF", 2);
  _loadStrings(_moveNames, 0x8247094, 13, "\x00", 1);
  _loadStrings(_abilityNames, 0x824FC4D, 13, "\x00", 1);
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
