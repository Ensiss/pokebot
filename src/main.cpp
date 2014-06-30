#include	<iostream>
#include	<SDL.h>
#include	"../vbam/gba/GBA.h"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"
#include	"Data.hh"
#include	"Action.hh"
#include	<list>

struct		Dump
{
  int		offset;
  uint8_t	size;
  uint8_t	*data;

  Dump(int off, int sz)
    : offset(off), size(sz), data(new uint8_t[size]())
  {
    uint32_t	o = *((uint32_t *) (internalRAM + offset));
    uint8_t	*ptr = (uint8_t *) (workRAM + o - WRAM_OFFSET);

    for (int i = 0; i < sz; i++)
      data[i] = ptr[i];
  }
};

void		doLoop()
{
  Data		data;
  Action	action(data);
  Team		&pTeam = data.playerTeam();
  Team		&eTeam = data.enemyTeam();
  int		step = 0;
  World::Path	*path = NULL;
  std::list<Dump>	addrs;
  int			scanlen = 80;
  bool			clear = true;

  while (emulating) {
    if (++step < 900)
      {
	sdlSetButton(KEY_BUTTON_AUTO_A, step < 899);
	sdlSetButton(KEY_BUTTON_SPEED, step < 899);
      }
    else if (step == 900)
      {
	for (int i = 0; i < 0x8000; i += 1)
	  {
	    uint32_t	offset = *((uint32_t *) (internalRAM + i));
	    if (offset > WRAM_OFFSET && offset < WRAM_OFFSET + 0x40000)
	      addrs.push_back(Dump(i, scanlen));
	  }
      }
    else
      {
	data.update();
	action.update();
	path = action.movement().getPath();
	if (step % 20 == 0)
	  {
	    if (clear)
	      printf("\033[2J\033[0;0H");
	    else
	      printf("\033[0;0H");
	    clear = false;

	    // for (int i = 0; i < 6; i++)
	    //   {
	    // 	printf("%s's (%d xp) %s", pTeam[i].getOtName(), pTeam[i].getXP(), data.name(pTeam[i].getSpecies()));
	    // 	printf("\tvs\t%s\n", eTeam[i].getNick());
	    //   }

	    // World::Map	&m = data.world()[data.player().getBank()][data.player().getMap()];
	    // for (int y = 0; y < m.height; y++)
	    //   {
	    // 	for (int x = 0; x < m.width; x++)
	    // 	  {
	    // 	    bool	node = false;
	    // 	    bool	ppos = (y == data.player().getY() && x == data.player().getX());
	    // 	    for (int a = 0; path && !node && a < path->size(); a++)
	    // 	      if (x == (*path)[a]->x && y == (*path)[a]->y)
	    // 		node = true;
	    // 	    printf("\033[1;%d;%dm%02x \033[0m", node && !ppos ? 47 : 40,
	    // 		   m.getMatterColor(m[y][x].status, ppos), m[y][x].status);
	    // 	  }
	    // 	printf("\n");
	    //   }

	    std::list<Dump>::iterator it = addrs.begin();
	    while (it != addrs.end())
	      {
		uint32_t			offset = *((uint32_t *) (internalRAM + it->offset));
		uint8_t				*ptr = (uint8_t *) (workRAM + offset - WRAM_OFFSET);
		std::list<Dump>::iterator	next = ++it;
		--it;

		if (offset < WRAM_OFFSET || offset >= WRAM_OFFSET + 0x40000)
		  {
		    addrs.erase(it);
		    clear = true;
		  }
		else
		  {
		    printf("%x -> %x: ", IRAM_OFFSET + it->offset, offset);
		    for (int i = 0; i < scanlen; i++)
		      {
			if (ptr[i] != it->data[i])
			  {
			    it->data[i] = ptr[i];
			    printf("\033[31m");
			  }
			printf("%02x\033[0m ", ptr[i]);
		      }
		    printf("\n");
		  }
		it = next;
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
