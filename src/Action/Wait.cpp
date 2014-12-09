#include	"Action/Wait.hh"
#include        "Lua.hh"

extern Lua      L;

/**
   Wait a certain number of frames

   @param frames        The duration of the wait
 */
Action::Wait::Wait(uint16_t frames)
  : _frames(frames), _until(NULL), _luaFunc(L.getState()), _useFrames(true)
{
}

/**
   Wait until a condition is met

   @param until         A function that returns true when the condition is met
 */
Action::Wait::Wait(bool (*until)())
  : _frames(0), _until(until), _luaFunc(L.getState()), _useFrames(false)
{
}

/**
   Wait until a condition is met

   @param until         A lua function that returns true when the condition is met
 */
Action::Wait::Wait(LuaRef until)
  : _frames(0), _until(NULL), _luaFunc(until), _useFrames(false)
{
}

Action::Wait::~Wait()
{
}

void		Action::Wait::_init()
{
}

void		Action::Wait::_update()
{
  if (_useFrames)
    {
      if (--_frames == 0)
	_state = Action::FINISHED;
    }
  else
    {
      if (_until && (*_until)())
	_state = Action::FINISHED;
      else if (_luaFunc.isFunction())
        {
          LuaRef        ret = _luaFunc();
          if (ret.type() == LUA_TBOOLEAN && ret.cast<bool>())
            _state = Action::FINISHED;
        }
    }
}
