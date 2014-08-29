#include	"Action/Wait.hh"

Action::Wait::Wait(uint16_t frames)
  : _frames(frames), _until(NULL), _useFrames(true)
{
}

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
