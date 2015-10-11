#include	"Action/AAction.hh"

AAction::AAction()
  : _data(*Data::data), _state(Action::NOT_STARTED), _counter(0)
{
  addListener("onInit", &AAction::_init);
  addListener("onUpdate", &AAction::_update);
}

AAction::~AAction()
{
  while (_actions.size())
    {
      delete _actions.front();
      _actions.pop();
    }
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
  emit("onFrame");
  if (_state == Action::RUNNING && !_actions.size())
    {
      emit("onUpdate");
      _counter++;
    }
  if (_state == Action::RUNNING && _actions.size())
    {
      _actions.front()->update();
      if (_actions.front()->getState() > Action::RUNNING)
	{
          for (uint8_t i = KEY_LEFT; i < KEY_BUTTON_SPEED; i++)
            sdlSetButton((EKey) i, false);
	  delete _actions.front();
	  _actions.pop();
	  if (!_actions.size())
	    emit("onEmptyQueue");
	}
    }
  return (_state);
}

void		AAction::emit(const std::string &signal)
{
  const auto          &it = _listeners.find(signal);

  if (it == _listeners.end())
    return;
  for (uint16_t i = 0; i < it->second._cppListeners.size(); i++)
    (it->second._cppListeners[i])(this);
}

void		AAction::addListener(const std::string &signal, void (*listener)(AAction *))
{
  _listeners[signal]._cppListeners.push_back(listener);
}

void		AAction::addListener(const std::string &signal, void (AAction::*listener)())
{
  std::function<void (AAction *)> l = [listener](AAction *that){(that->*listener)();};

  _listeners[signal]._cppListeners.push_back(l);
}
