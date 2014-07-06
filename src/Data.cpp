#include	"Data.hh"

Data::Data()
  : _pteam((uint64_t) (workRAM + PTEAM_PTR)),
    _eteam((uint64_t) (workRAM + ETEAM_PTR))
{
  _loadNames();
}

Data::~Data()
{
  for (int i = 0; i < _names.size(); i++)
    delete _names[i];
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

  for (int id = 0; 1; id++)
    {
      _names.push_back(new char[11]());
      ptr = rom + NAMES_PTR + id * 11;
      for (int i = 0; i < 11; i++)
	_names[id][i] = (i < 10 && ptr[i] != 0xFF) * pokeCharsetToAscii(ptr[i]);
      if (_names[id][0] == '-' && _names[id][1] == 0)
	break;
    }
}
