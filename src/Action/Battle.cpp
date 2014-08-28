#include	"ActionBattle.hh"

Action::Battle::Battle()
  : _tick(0)
{
}

Action::Battle::~Battle()
{
}

void		Action::Battle::_releaseKeys()
{
  for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
    if (i != KEY_BUTTON_SPEED)
      sdlSetButton((EKey) i, false);
}

uint8_t		Action::Battle::_getBestMove()
{
  const BattleData	&p = _data.battlers()[0];
  const BattleData	&e = _data.battlers()[1];
  const Species	&sp = _data.species(p.getSpecies());
  uint8_t	best = 0;
  uint8_t	min = 0;

  for (int m = 0; m < 4; m++)
    {
      if (p.getMove(m) && p.getPP(m))
	{
	  const Move	&move = _data.move(p.getMove(m));
	  Range		dmg = _data.potentialDamage(p, e, move);

	  if ((min < e.getHP() && dmg.min > min) || (dmg.min > e.getHP() && dmg.min < min))
	    {
	      best = m;
	      min = dmg.min;
	    }
	}
    }
  return (best);
}

void		Action::Battle::_selectMove(uint8_t move, uint8_t curr)
{
  if (curr <= 1 && move > 1)
    sdlSetButton(KEY_DOWN, true);
  else if (curr > 1 && move <= 1)
    sdlSetButton(KEY_UP, true);
  else if (curr < move)
    sdlSetButton(KEY_RIGHT, true);
  else if (curr > move)
    sdlSetButton(KEY_LEFT, true);
}

void		Action::Battle::_init()
{
}

void		Action::Battle::_update()
{
  BattleMenu	&bm = _data.battleMenu();

  _releaseKeys();
  if (_tick = !_tick)
    return;
  if (bm.isOpen())
    {
      if (bm.getMenu() == 0)
	{
	  if (bm.getCursor() == 0)
	    sdlSetButton(KEY_BUTTON_A, true);
	}
      else if (bm.getMenu() == 1)
	{
	  uint8_t	move = _getBestMove();

	  if (bm.getAttack() == move)
	    sdlSetButton(KEY_BUTTON_A, true);
	  else
	    _selectMove(move, bm.getAttack());
	}
    }
  else
    sdlSetButton(KEY_BUTTON_A, true);
}
