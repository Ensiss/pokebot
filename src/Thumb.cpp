#include	"Thumb.hh"

void		Thumb::disass(uint32_t addr, uint8_t rec)
{
  bool		first = true;
  char		ins[128];
  uint32_t	next;
  uint32_t	jump;
  char		*ptr;

  addr &= ~1;
  do {
    next = addr + disThumb(addr, ins, 0);
    for (int i = 0; i < 2 * rec; i++)
      printf(" ");
    printf("%s", ins);
    if (first)
      printf("\t\t// at 0x%08x", addr);
    printf("\n");
    if (ins[0] == 'b' && (ptr = strchr(ins, '$')))
      {
	sscanf(ptr + 1, "%x", &jump);
	disass(jump, rec + (ins[1] != ' '));
	if (ins[1] == ' ')
	  return;
      }
    addr = next;
    first = false;
  } while (ins[0] != 'b' || ins[1] != 'x');
}

void		Thumb::disassPokeScript(uint8_t instr)
{
  uint32_t	off = *((uint32_t *) gbaMem(0x0815F9B4 + (instr << 2)));

  printf("\n");
  disass(off);
}
