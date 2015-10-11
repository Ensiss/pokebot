#include	"Action/MoveCursor.hh"
#include        "Lua.hh"

extern Lua      L;

/**
   Moves the cursor to a specific position in a box

   @param w     Width of the box
   @param h     Height of the box
   @param dest  Index of destination position
   @param f     Function that returns the current cursor position
 */
Action::MoveCursor::MoveCursor(uint8_t w, uint8_t h, uint8_t dest, uint8_t (*f)())
  : _w(w), _h(h), _dx(dest % w), _dy(dest / w), _f(f), _luaFunc(L.getState())
{
}

/**
   Moves the cursor to a specific position in a box

   @param w     Width of the box
   @param h     Height of the box
   @param dest  Index of destination position
   @param f     Lua function that returns the current cursor position
 */
Action::MoveCursor::MoveCursor(uint8_t w, uint8_t h, uint8_t dest, LuaRef f)
  : _w(w), _h(h), _dx(dest % w), _dy(dest / w), _f(NULL), _luaFunc(f)
{
}

Action::MoveCursor::~MoveCursor()
{
}

void		Action::MoveCursor::_init()
{
}

void		Action::MoveCursor::_update()
{
  static int	tick = 0;
  int           cx, cy;
  int		c;

  if (_f)
    c = (*_f)();
  else if (_luaFunc.isFunction())
    c = _luaFunc().cast<int>();
  else
    {
      _state = Action::ERROR;
      return;
    }
  cx = c % _w;
  cy = c / _w;

  for (uint8_t i = KEY_LEFT; i <= KEY_BUTTON_AUTO_B; i++)
    if (i != KEY_BUTTON_SPEED)
      sdlSetButton((EKey) i, false);
  if ((tick = !tick))
    return;
  if (cx == _dx && cy == _dy)
    {
      _state = Action::FINISHED;
      return;
    }
  if (cx > _dx)
    sdlSetButton(KEY_LEFT, true);
  else if (cy > _dy)
    sdlSetButton(KEY_UP, true);
  else if (cx < _dx)
    sdlSetButton(KEY_RIGHT, true);
  else if (cy < _dy)
    sdlSetButton(KEY_DOWN, true);
}
