#include	"Action/Wait.hh"

/**
   Wait a certain number of frames

   @param frames        The duration of the wait
 */
Action::Wait::Wait(uint16_t frames)
  : _frames(frames), _until(NULL), _useFrames(true)
{
}

/**
   Wait until a condition is met

   @param until         A function that returns true when the condition is met
 */
Action::Wait::Wait(bool (*until)())
  : _frames(0), _until(until), _useFrames(false)
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
      if ((*_until)())
	_state = Action::FINISHED;
    }
}
