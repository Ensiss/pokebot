#ifndef		__SDLGLOBALS_H__
#define		__SDLGLOBALS_H__

#include	"inputSDL.h"
#include	"Data.hh"

class	Bot;

void	sdlSetButton(EKey key, bool state);
void    sdlPressButton(EKey key);
void	sdlReleaseButton(EKey key);
bool	sdlGetButton(EKey key);
void	initVBAM(int argc, char **argv);
void	destroyVBAM();
void	sdlPollEvents(Data &data, Bot &bot);

extern int pauseWhenInactive;
extern int active;
extern int emulating;
extern bool paused;
extern struct EmulatedSystem emulator;

#endif
