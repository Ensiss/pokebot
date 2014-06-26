#ifndef		__POKEMONUTILS_HH__
#define		__POKEMONUTILS_HH__

#include	<stdint.h>

#define		ROM_OFFSET	0x8000000
#define		BANK_PTR	0x3526A8
#define		NAMES_PTR	0x245EE0
#define		PTEAM_PTR	0x024284
#define		ETEAM_PTR	0x02402C

char		pokeCharsetToAscii(uint8_t c);

#endif
