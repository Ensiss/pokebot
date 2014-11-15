#ifndef         __MULTICHOICE_HH__
#define         __MULTICHOICE_HH__

#include	"PokemonUtils.hh"

class           MultiChoice
{
public:
  MultiChoice() {}

public:
  uint8_t       getNbChoices() const { return (_nbChoices); }
  uint32_t      getChoicePtr(uint8_t choice) const { return (((uint32_t *) gbaMem(_strTable))[choice * 2]); }

private:
  uint32_t      _strTable;
  uint8_t       _nbChoices;
  uint8_t       _padding[3];
};

#endif
