#ifndef         __VM_HH__
#define         __VM_HH__

#include        <cstring>
#include        "Script.hh"

#define         VM_FLAGS        0x900
#define         VM_VARS         0x100
#define         VM_TEMP         0x0F
#define         VM_BANKS        0x04
#define         VM_BUFF         0x03
#define         VM_VAR_OFFSET   0x4000
#define         VM_TEMP_OFFSET  0x8000

#define         VM_IS_FLAG(x)   ((x) < VM_FLAGS)
#define         VM_IS_BANK(x)   ((x) < VM_BANKS)
#define         VM_IS_VAR(x)    ((x) >= VM_VAR_OFFSET && (x) < VM_VAR_OFFSET + VM_VARS)
#define         VM_IS_TEMP(x)   ((x) >= VM_TEMP_OFFSET && (x) < VM_TEMP_OFFSET + VM_TEMP)

#define         VM_BOUNDS_ERR(x)        ("Warning: " x " 0x%x out of bounds\n")

class           VM
{
public:
  VM();
  ~VM();

public:
  void          update();
  bool          getFlag(uint16_t flag);
  uint16_t      getVar(uint16_t var);
  uint32_t      getBank(uint8_t bank);
  void          setFlag(uint16_t flag, bool val);
  void          setVar(uint16_t var, uint16_t val);
  void          setBank(uint8_t bank, uint32_t val);

private:
  uint8_t       _flags[VM_FLAGS >> 3];
  uint16_t      _vars[VM_VARS];
  uint16_t      _temp[VM_TEMP];
  uint32_t      _banks[VM_BANKS];
};

#endif
