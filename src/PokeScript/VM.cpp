#include        "VM.hh"

VM::VM()
{
  update();
}

VM::~VM()
{
}

void            VM::update()
{
  uint32_t      ptr = *((uint32_t *) gbaMem(0x03005008));
  uint8_t       *flags = (uint8_t *) gbaMem(ptr + 0xEE0);
  uint16_t      *vars = (uint16_t *) gbaMem(ptr + 0x1000);

  memcpy(_flags, flags, sizeof(_flags));
  memcpy(_vars, vars, sizeof(_vars));
  memset(_temp, 0, sizeof(_temp));
  memset(_banks, 0, sizeof(_banks));
}

bool            VM::getFlag(uint16_t id)
{
  if (VM_IS_FLAG(id))
    return (_flags[id >> 3] & (1 << (id % 8)));
  fprintf(stderr, "Warning: flag 0x%x out of bounds\n", id);
  return (0);
}

uint16_t        VM::getVar(uint16_t id)
{
  if (VM_IS_VAR(id))
    return (_vars[id - VM_VAR_OFFSET]);
  if (VM_IS_TEMP(id))
    return (_temp[id - VM_TEMP_OFFSET]);
  fprintf(stderr, "Warning: variable 0x%x out of bounds\n", id);
  return (0);
}

void            VM::setFlag(uint16_t id, bool val)
{
  if (!VM_IS_FLAG(id))
    {
      fprintf(stderr, "Warning: flag 0x%x out of bounds\n", id);
      return;
    }
  if (val)
    _flags[id >> 3] |= (1 << (id % 8));
  else
    _flags[id >> 3] &= ~(1 << (id % 8));
}

void            VM::setVar(uint16_t id, uint16_t val)
{
  if (VM_IS_VAR(id))
    _vars[id - VM_VAR_OFFSET] = val;
  else if (VM_IS_TEMP(id))
    _temp[id - VM_TEMP_OFFSET] = val;
  else
    fprintf(stderr, "Warning: variable 0x%x out of bounds\n", id);
}
