#include	"AAction.hh"

AAction::AAction()
  : _data(*Action::data), _state(Action::NOT_STARTED)
{
}

Action::State	AAction::update()
{
  if (_state == Action::NOT_STARTED)
    {
      _init();
      if (_state == Action::NOT_STARTED)
	_state = Action::RUNNING;
    }
  if (_state == Action::RUNNING && !_actions.size())
    _update();
  if (_state == Action::RUNNING && _actions.size())
    {
      _actions.front()->update();
      if (_actions.front()->getState() > Action::RUNNING)
	_actions.pop();
    }
  return (_state);
}
