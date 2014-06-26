#include	"Data.hh"

Data::Data()
  : _pteam((uint64_t) (workRAM + 0x24284)),
    _eteam((uint64_t) (workRAM + 0x2402C))
{
  _loadNames();
}

Data::~Data()
{
}

void		Data::update()
{
  _pteam.update();
  _eteam.update();
}

void		Data::_loadNames()
{
  uint8_t	*ptr;

  _names = new char*[153]();
  for (int id = 0; id <= 151; id++)
    {
      _names[id] = new char[11]();
      ptr = rom + 0x245EE0 + id * 11;
      for (int i = 0; i < 11; i++)
	_names[id][i] = (i < 10 && ptr[i] != 0xFF) * pokeCharsetToAscii(ptr[i]);
    }
  _names[152] = NULL;
}
