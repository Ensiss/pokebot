#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"

void		doLoop()
{
  Data		data;
  Team		&pTeam = data.playerTeam();
  Team		&eTeam = data.enemyTeam();
  int		step = 0;
  std::vector<World::Map::Node*>	*path = NULL;
  bool		pushed = false;
  bool		oldState = false;
  uint16_t	oldx = 0;
  uint16_t	oldy = 0;
  int		oldstep;
  uint8_t	pathi = 0;

  while (emulating) {
    if (++step < 900)
      {
	sdlSetButton(KEY_BUTTON_AUTO_A, step < 899);
	sdlSetButton(KEY_BUTTON_SPEED, step < 899);
      }
    else if (step % 20 == 0)
      {
	data.update();
	printf("\033[2J\033[0;0H");
	for (int i = 0; i < 6; i++)
	  {
	    printf("%s's (%d xp) %s", pTeam[i].getOtName(), pTeam[i].getXP(), data.name(pTeam[i].getSpecies()));
	    printf("\tvs\t%s\n", eTeam[i].getNick());
	  }
	World::Map	&m = data.world()[data.player().getBank()][data.player().getMap()];
	if (pushed)
	  {
	    if (path)
	      delete path;
	    path = m.findPath(data.player().getX(), data.player().getY(), 2, 2);
	    pathi = 1;
	    pushed = false;
	  }
	for (int y = 0; y < m.height; y++)
	  {
	    for (int x = 0; x < m.width; x++)
	      {
		bool	node = false;
		bool	ppos = (y == data.player().getY() && x == data.player().getX());
		for (int a = 0; path && !node && a < path->size(); a++)
		  if (x == (*path)[a]->x && y == (*path)[a]->y)
		    node = true;
		printf("\033[1;%d;%dm%02x \033[0m", node && !ppos ? 47 : 40,
		       m.getMatterColor(m[y][x].status, ppos), m[y][x].status);
	      }
	    printf("\n");
	  }
      }

    if (path && pathi == path->size() && (oldx != data.player().getX() || oldy != data.player().getY()))
      {
	for (uint8_t i = KEY_LEFT; i <= KEY_DOWN; i++)
	  sdlSetButton((EKey) i, false);
	delete path;
	path = NULL;
      }
    if (path && pathi < path->size())
      {
    	if (pathi == 1 || (oldx != data.player().getX() || oldy != data.player().getY()))
    	  {
    	    for (uint8_t i = KEY_LEFT; i <= KEY_DOWN; i++)
    	      sdlSetButton((EKey) i, false);
	    int		dx = (*path)[pathi]->x - data.player().getX();
	    int		dy = (*path)[pathi]->y - data.player().getY();
	    EKey	k;
	    if (!dx)
	      k = dy < 0 ? KEY_UP : KEY_DOWN;
	    else
	      k = dx < 0 ? KEY_LEFT : KEY_RIGHT;
	    sdlSetButton(k, true);
	    pathi++;
	    oldstep = step;
    	  }
      }
    if (step > 900)
      {
	oldx = data.player().getX();
	oldy = data.player().getY();
      }

    emulator.emuMain(emulator.emuCount);
    sdlPollEvents();
    if (oldState && !sdlGetButton(KEY_BUTTON_B))
      pushed = true;
    oldState = sdlGetButton(KEY_BUTTON_B);
  }
}

int		main(int ac, char **av)
{
  if (ac < 2)
    {
      std::cerr << "Usage: ./pokebot <Pokemon_FireRed.gba>" << std::endl;
      return (1);
    }
  initVBAM(ac, av);
  doLoop();
  destroyVBAM();
  return (0);
}
