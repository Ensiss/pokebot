#include	"Data.hh"

Data::Data()
  : _pteam((uint64_t) (workRAM + PTEAM_PTR)),
    _eteam((uint64_t) (workRAM + ETEAM_PTR))
{
  _loadNames();
}

Data::~Data()
{
}

void		Data::update()
{
  _player.update();
  _pteam.update();
  _eteam.update();
}

void		Data::_loadNames()
{
  uint8_t	*ptr;

  _names = new char*[440]();
  for (int id = 0; id <= 438; id++)
    {
      _names[id] = new char[11]();
      ptr = rom + NAMES_PTR + id * 11;
      for (int i = 0; i < 11; i++)
	_names[id][i] = (i < 10 && ptr[i] != 0xFF) * pokeCharsetToAscii(ptr[i]);
    }
  _names[440] = NULL;
}
