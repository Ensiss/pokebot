#include        "VM.hh"

VM::Context::Context()
{
  update();
}

VM::Context::Context(const Context &ctx)
{
  (*this) = ctx;
}

VM::Context     &VM::Context::operator=(const Context &ctx)
{
  cpts.choices = ctx.cpts.choices;
  stack = ctx.stack;
  pc = ctx.pc;
  cmp1 = ctx.cmp1;
  cmp2 = ctx.cmp2;
  cpts.pts = ctx.cpts.pts;
  memcpy(_flags, ctx._flags, sizeof(_flags));
  memcpy(_vars, ctx._vars, sizeof(_vars));
  memcpy(_temp, ctx._temp, sizeof(_temp));
  memcpy(_banks, ctx._banks, sizeof(_banks));
  return (*this);
}

void            VM::Context::update()
{
  uint32_t      ptr = *((uint32_t *) gbaMem(0x03005008));
  uint8_t       *flags = (uint8_t *) gbaMem(ptr + 0xEE0);
  uint16_t      *vars = (uint16_t *) gbaMem(ptr + 0x1000);

  memcpy(_flags, flags, sizeof(_flags));
  memcpy(_vars, vars, sizeof(_vars));
  memset(_temp, 0, sizeof(_temp));
  memset(_banks, 0, sizeof(_banks));
  cpts.pts = 0;
  cpts.choices.clear();
  clearStack();
}

bool            VM::Context::getFlag(uint16_t id) const
{
  if (VM_IS_FLAG(id))
    return (_flags[id >> 3] & (1 << (id % 8)));
  fprintf(stderr, VM_BOUNDS_ERR("flag"), id);
  return (0);
}

uint16_t        VM::Context::getVar(uint16_t id) const
{
  if (VM_IS_VAR(id))
    return (_vars[id - VM_VAR_OFFSET]);
  if (VM_IS_TEMP(id))
    return (_temp[id - VM_TEMP_OFFSET]);
  fprintf(stderr, VM_BOUNDS_ERR("variable"), id);
  return (0);
}

uint32_t        VM::Context::getBank(uint8_t id) const
{
  if (VM_IS_BANK(id))
    return (_banks[id]);
  fprintf(stderr, VM_BOUNDS_ERR("bank"), id);
  return (0);
}

void            VM::Context::setFlag(uint16_t id, bool val)
{
  if (!VM_IS_FLAG(id))
    {
      fprintf(stderr, VM_BOUNDS_ERR("flag"), id);
      return;
    }
  if (val)
    _flags[id >> 3] |= (1 << (id % 8));
  else
    _flags[id >> 3] &= ~(1 << (id % 8));
}

void            VM::Context::setVar(uint16_t id, uint16_t val)
{
  if (VM_IS_VAR(id))
    _vars[id - VM_VAR_OFFSET] = val;
  else if (VM_IS_TEMP(id))
    _temp[id - VM_TEMP_OFFSET] = val;
  else
    fprintf(stderr, VM_BOUNDS_ERR("variable"), id);
}

void            VM::Context::setBank(uint8_t id, uint32_t val)
{
  if (VM_IS_BANK(id))
    _banks[id] = val;
  else
    fprintf(stderr, VM_BOUNDS_ERR("bank"), id);
}
