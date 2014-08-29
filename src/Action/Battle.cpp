#include	"Action/Battle.hh"

Action::Battle::Battle()
{
}

Action::Battle::~Battle()
{
}

uint8_t		Action::Battle::_getBestMove()
{
  const BattleData	&p = _data.battlers()[0];
  const BattleData	&e = _data.battlers()[1];
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

void		Action::Battle::_init()
{
}

void		Action::Battle::_update()
{
  BattleMenu	&bm = _data.battleMenu();

  if (bm.isOpen() && bm.getMenu() == 0)
    {
      queue(new Action::MoveCursor(2, 2, 0, []() -> uint8_t { return (Action::data->battleMenu().getCursor()); }));
      queue(new Action::PressButton(KEY_BUTTON_A));
      queue(new Action::MoveCursor(2, 2, _getBestMove(), []() -> uint8_t { return (Action::data->battleMenu().getAttack()); }));
      queue(new Action::PressButton(KEY_BUTTON_A));
    }
  else
    queue(new Action::PressButton(KEY_BUTTON_A));
}
