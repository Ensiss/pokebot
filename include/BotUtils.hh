#ifndef         __BOTUTILS_HH__
#define         __BOTUTILS_HH__

#include        "Bot.hh"
#include        "Action/MoveTo.hh"
#include        "Action/TalkTo.hh"
#include        "Action/UseWarp.hh"
#include        "Action/PressButton.hh"

namespace       BotUtils
{
  void          queue(AAction *act);
  void          clear();
  void          moveTo(uint16_t x, uint16_t y);
  void          talkTo(uint8_t id);
  void          useWarp(uint8_t id);
  void          pressButton(uint8_t key);
  void          wait(uint16_t frames);
};

#endif
