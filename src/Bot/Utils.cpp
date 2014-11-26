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

void            BotUtils::moveTo(uint16_t x, uint16_t y)
{
  bot.queue(new Action::MoveTo(x, y));
}

void            BotUtils::talkTo(uint8_t id)
{
  bot.queue(new Action::TalkTo(id));
}

void            BotUtils::useWarp(uint8_t id)
{
  bot.queue(new Action::UseWarp(id));
}

void            BotUtils::pressButton(uint8_t key)
{
  bot.queue(new Action::PressButton((EKey) key));
}

void            BotUtils::wait(uint16_t frames)
{
  bot.queue(new Action::Wait(frames));
}
