#include	"Data.hh"
#include        "Script.hh"
#include        "PrintUtils.hh"

Data            *Data::data = NULL;

Data::Data()
  : _ows(gbaPtr<OverWorld *>(0x02036E38)),
    _mchoice(gbaPtr<MultiChoice *>(0x083E04B0)),
    _pteam(0x02024284),
    _eteam(0x0202402C),
    _battlers(0x02023BE4)
{
  _loadStrings(_speciesNames, 0x8245EE0, 11, "\xAE\xFF", 2);
  _loadStrings(_moveNames, 0x8247094, 13, "\x00", 1);
  _loadStrings(_abilityNames, 0x824FC4D, 13, "\x00", 1);
  _loadStrings(_typeNames, 0x824F1A0, 7, "\x00", 1);
  _loadMoves();
  _loadSpecies();
  _loadItems();
  _loadTypeEffectiveness();
}

Data::~Data()
{
  for (uint16_t i = 0; i < _speciesNames.size(); i++)
    delete _speciesNames[i];
  for (uint16_t i = 0; i < _moveNames.size(); i++)
    delete _moveNames[i];
  for (uint16_t i = 0; i < _abilityNames.size(); i++)
    delete _abilityNames[i];
}

void		Data::update()
{
  uint8_t memb = gbaMem<uint8_t>(0x02031DBC);
  uint8_t memm = gbaMem<uint8_t>(0x02031DBD);
  static int    bank = 0;
  static int    map = 0;
  uint8_t       newb, newm;

  _player.update();
  _pteam.update();
  _eteam.update();
  _battlers.update();
  newb = _player.getBankId();
  newm = _player.getMapId();
  if (!_player.isValid() || (bank && map && (memb != newb || memm != newm)))
    return;
  if (newb != 255 && newm != 255 && (newb != bank || newm != map || (!bank && !map)))
    {
      // Try to detect if player data is corrupted
      // This is NOT safe and needs to be changed to something better
      const std::vector<std::vector<World::Map> > _banks = _world.getBanks();
      if (newb >= _banks.size() || newm >= _banks[newb].size())
        return;
      World::Map &m = _world.getMap(newb, newm);
      if (_player.getX() >= m.getWidth() || _player.getY() >= m.getHeight())
        return;
      // --------------------------------------------------

      bank = newb;
      map = newm;
      for (uint8_t i = 0; i < m.getNbPersons(); i++)
        Script::getPerson(i);
    }
}

float		Data::typeEffectiveness(const Move &m, const Species &sp) const
{
  float		eff = typeEffectiveness(m.getTypeId(), sp.getTypeId(0));

  if (sp.getTypeId(0) != sp.getTypeId(1))
    eff *= typeEffectiveness(m.getTypeId(), sp.getTypeId(1));
  return (eff);
}

float		Data::sameTypeAttackBonus(const Move &m, const Species &s) const
{
  uint8_t	mt = m.getTypeId();
  uint8_t	*st = s.getTypesId();

  return (1 + 0.5 * (st[0] == mt || st[1] == mt));
}

Range		Data::potentialDamage(const IPokeData &attacker, const IPokeData &target, const Move &m) const
{
  const Species	&as = getSpecies(attacker.getSpeciesId());
  const Species	&ts = getSpecies(target.getSpeciesId());
  float		a = attacker.getLevel();
  float		b = isSpecial(m.getTypeId()) ? attacker.getRealSpAtk() : attacker.getRealAtk();
  float		c = m.getPower();
  float		d = isSpecial(m.getTypeId()) ? target.getRealSpDef() : target.getRealDef();
  float		x = sameTypeAttackBonus(m, as);
  float		y = typeEffectiveness(m, ts);
  int		dmg;

  if (!target.getSpeciesId() || !attacker.getSpeciesId() || !m.getPower())
    return (Range());
  dmg = 2.0 * a / 5.0 + 2;
  dmg = (dmg * b * c) / d;
  dmg = (dmg / 50.0) + 2;
  dmg = dmg * x * y;
  return (Range((dmg * 217) / 255, dmg));
}

float		Data::chanceToHit(const IPokeData &attacker, const IPokeData &target, const Move &m) const
{
  return (m.getAccuracy() * ((float) attacker.getRealAccuracy() / (float) target.getRealEvasion()));
}

void		Data::_loadStrings(std::vector<char *> &dest, uint32_t addr, uint8_t len, const char* delim, uint8_t delimsz)
{
  int		i, id;
  uint8_t	*ptr;

  for (id = 0; id >= 0; id++)
    {
      ptr = gbaPtr<uint8_t *>(addr) + id * len;
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

void		Data::_loadItems()
{
  for (uint16_t i = 0; i < 375; i++)
    _items.push_back(Item(0x083DB028 + i * 44));
}

void		Data::_loadTypeEffectiveness()
{
  for (int j = 0; j < 18; j++)
    for (int i = 0; i < 18; i++)
      _typeChart[j][i] = 1.0;
  for (uint8_t *ptr = gbaPtr<uint8_t *>(0x0824F050); *ptr != 0xFF; ptr += 3)
    {
      if (ptr[0] < 18)
	{
	  float	val = ptr[2] == 0x14 ? 2.0 : ptr[2] == 0x05 ? 0.5 : 0.0;
	  _typeChart[ptr[0]][ptr[1]] = val;
	}
    }
}
