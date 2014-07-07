#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"
#include	"Action.hh"
#include	"PokeScript.hh"
#include	<list>

void		printTeam(Team &pTeam, Team &eTeam, Data &data)
{
  for (int i = 0; i < 6; i++)
    {
      printf("%s's (%d xp) %s",
	     pTeam[i].getOtName(), pTeam[i].getXP(),
	     data.speciesName(pTeam[i].getSpecies()));
      printf("\tvs\t%s\n", eTeam[i].getNick());
    }
}

void		doLoop()
{
  Data		data;
  Action	action(data);
  Team		&pTeam = data.playerTeam();
  Team		&eTeam = data.enemyTeam();
  int		step = 0;
  World::Path	*path = NULL;

  while (emulating) {
    if (++step < 900)
      {
	sdlSetButton(KEY_BUTTON_AUTO_A, step < 899);
	sdlSetButton(KEY_BUTTON_SPEED, step < 899);
      }
    else
      {
	data.update();
	action.update();
	path = action.movement().getPath();
	if (step % 20 == 0)
	  {
	    printf("\033[2J\033[0;0H");
	    printTeam(pTeam, eTeam, data);

	    World::Map	&m = data.world()[data.player().getBank()][data.player().getMap()];
	    for (int y = 0; y < m.height; y++)
	      {
	    	for (int x = 0; x < m.width; x++)
	    	  {
	    	    bool	node = false;
	    	    bool	ppos = (y == data.player().getY() && x == data.player().getX());
	    	    for (int a = 0; path && !node && a < path->size(); a++)
	    	      if (x == (*path)[a]->x && y == (*path)[a]->y)
	    		node = true;
		    for (int a = 0; !node && a < m.nbWarps; a++)
		      if (m.warps[a].x == x && m.warps[a].y == y)
			node = true;
		    printf("\033[1;%d;%dm%02x \033[0m", node && !ppos ? 47 : 40,
			   m.getMatterColor(m[y][x].status, ppos), m[y][x].status);
		    printf("\033[1;%d;%dm%02x \033[0m", node && !ppos ? 47 : 40,
			   m.getMatterColor(m[y][x].status, ppos), m[y][x].attr->behavior);
	    	  }
	    	printf("\n");
	      }
	  }
      }

    emulator.emuMain(emulator.emuCount);
    sdlPollEvents(data, action);
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
