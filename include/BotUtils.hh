#ifndef         __BOTUTILS_HH__
#define         __BOTUTILS_HH__

#include        <LuaBridge.h>
#include        "Bot.hh"
#include        "Action/MoveTo.hh"
#include        "Action/TalkTo.hh"
#include        "Action/UseWarp.hh"
#include        "Action/PressButton.hh"
#include        "Action/LuaClass.hh"

namespace       BotUtils
{
  void          queue(AAction *act);
  void          clear();
};

#endif
