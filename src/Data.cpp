#include	"Data.hh"

Data::Data()
  : _pteam((uint64_t) (workRAM + PTEAM_PTR)),
    _eteam((uint64_t) (workRAM + ETEAM_PTR))
{
  _loadNames();
  _loadMoveNames();
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
      if (ptr[0] == 0xAE && ptr[1] == 0xFF)
	break;
      for (int i = 0; i < 11; i++)
	_names[id][i] = (i < 10 && ptr[i] != 0xFF) * pokeCharsetToAscii(ptr[i]);
    }
}

void		Data::_loadMoveNames()
{
  uint8_t	*ptr;
  int		len = 13;

  for (int id = 0; 1; id++)
    {
      _moveNames.push_back(new char[len]());
      ptr = rom + MOVE_NAMES_PTR + id * len;
      if (ptr[0] == 0x00)
	break;
      for (int i = 0; i < len; i++)
	_moveNames[id][i] = (i < len - 1 && ptr[i] != 0xFF) * pokeCharsetToAscii(ptr[i]);
    }
}
