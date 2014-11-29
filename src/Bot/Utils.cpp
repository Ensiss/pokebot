#include        "BotUtils.hh"

extern Bot      bot;

void            BotUtils::queue(AAction *act)
{
  bot.queue(act);
}

void            BotUtils::clear()
{
  bot.clear();
}
