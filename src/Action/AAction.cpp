#include	"Action/AAction.hh"

AAction::AAction()
  : _data(*Action::data), _state(Action::NOT_STARTED)
{
  addListener("onInit", &AAction::_init);
  addListener("onUpdate", &AAction::_update);
}

Action::State	AAction::update()
{
  if (_state == Action::NOT_STARTED)
    {
      while (_actions.size())
	{
	  delete _actions.front();
	  _actions.pop();
	}
      emit("onInit");
      if (_state == Action::NOT_STARTED)
	_state = Action::RUNNING;
    }
  if (_state == Action::RUNNING && !_actions.size())
    emit("onUpdate");
  if (_state == Action::RUNNING && _actions.size())
    {
      _actions.front()->update();
      if (_actions.front()->getState() > Action::RUNNING)
	{
	  delete _actions.front();
	  _actions.pop();
	}
    }
  return (_state);
}

void		AAction::emit(const std::string &signal)
{
  std::map<std::string, std::vector<std::function<void(AAction *)> > >::iterator	it;

  if ((it = _listeners.find(signal)) == _listeners.end())
    return;
  for (uint16_t i = 0; i < it->second.size(); i++)
    (it->second[i])(this);
}

void		AAction::addListener(const std::string &signal, void (*listener)(AAction *))
{
  _listeners[signal].push_back(listener);
}

void		AAction::addListener(const std::string &signal, void (AAction::*listener)())
{
  std::function<void (AAction *)> l = [listener](AAction *that){(that->*listener)();};

  _listeners[signal].push_back(l);
}
