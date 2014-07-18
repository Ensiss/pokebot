#ifndef		__PRINT_UTILS_HH__
#define		__PRINT_UTILS_HH__

#include	<stdint.h>
#include	"Data.hh"
#include	"Action.hh"
#include	"PokemonUtils.hh"

void		printTeam(Data &data);
void		printMap(Data &data, Action &action);
void		printMenu(Data &data);
void		printRAM(Data &data, uint32_t address, uint32_t sz, int linesz = 0);
void		printMessageBoxes(int offsetx = 0, int offsety = 0);
void		printString(uint32_t addr, uint32_t size);
void		searchString(uint32_t start, uint32_t size, const char *m);

#endif
