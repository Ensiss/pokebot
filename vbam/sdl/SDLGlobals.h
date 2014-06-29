#ifndef		__SDLGLOBALS_H__
#define		__SDLGLOBALS_H__

#include	"inputSDL.h"
#include	"Data.hh"

class	Action;

void	sdlSetButton(EKey key, bool state);
bool	sdlGetButton(EKey key);
void	initVBAM(int argc, char **argv);
void	destroyVBAM();
void	sdlPollEvents(Data &data, Action &action);

extern int pauseWhenInactive;
extern int active;
extern int emulating;
extern bool paused;
extern struct EmulatedSystem emulator;

#endif
