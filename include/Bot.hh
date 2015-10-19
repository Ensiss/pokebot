#ifndef		__BOT_HH__
#define		__BOT_HH__

#include	<deque>
#include	"Data.hh"
#include	"Action/AAction.hh"
#include	"../vbam/sdl/SDLGlobals.h"
#include        <LuaBridge.h>

using namespace luabridge;

class		Bot
{
public:
  Bot();
  ~Bot();

public:
  void		update();
  void		queue(LuaRef action);
  void		clear();

private:
  std::deque<LuaRef>	_queue;

public:
  static Bot            *getBotWrapper() { return &bot; }

public:
  static Bot            bot;
};

#endif
