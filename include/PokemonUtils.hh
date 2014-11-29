#ifndef		__POKEMONUTILS_HH__
#define		__POKEMONUTILS_HH__

#include	<stdint.h>
#include	"../vbam/gba/Globals.h"

#define		WRAM_OFFSET	0x2000000
#define		IRAM_OFFSET	0x3000000
#define		ROM_OFFSET	0x8000000

#define		BANK_PTR	0x3526A8
#define		NAMES_PTR	0x245EE0
#define		MOVE_NAMES_PTR	0x247094
#define		PTEAM_PTR	0x024284
#define		ETEAM_PTR	0x02402C
#define		CURR_BANK_PTR	0x031DBC
#define		CURR_MAP_PTR	0x031DBD

#define		POW(x)		((x) * (x))

char		pokeCharsetToAscii(uint8_t c);
bool            getFlag(uint16_t flag);
uint16_t        getVar(uint16_t var);
void		*gbaMem(uint32_t ptr);

template <typename T>
T               gbaMem(uint32_t ptr)
{
  return (*((T *) gbaMem(ptr)));
}

#endif
