#ifndef		__AACTION_HH__
#define		__AACTION_HH__

#include	<map>
#include	<queue>
#include	<vector>
#include	<string>
#include	<functional>
#include        <LuaBridge.h>
#include	"Data.hh"
#include	"../vbam/sdl/SDLGlobals.h"

using namespace luabridge;

class           AAction;

namespace	Action
{
  enum		State
    {
      NOT_STARTED,
      RUNNING,
      FINISHED,
      ERROR
    };

  struct        Listeners
  {
    std::vector<std::function<void (AAction *)> > _cppListeners;
    std::vector<LuaRef *> _luaListeners;
  };
};

class		AAction
{
public:
  AAction();
  Action::State	update();
  virtual	~AAction();

private:
  virtual void	_update() = 0;
  virtual void	_init() = 0;

public:
  void		queue(AAction *act) { _actions.push(act); }
  void		reset() { _state = Action::NOT_STARTED; }
  Action::State	getState() const { return (_state); }
  void		setState(Action::State st) { _state = st; }
  uint32_t      getCounter() const { return (_counter); }

public:
  void		emit(const std::string &signal);
  void		addListener(const std::string &signal, void (*listener)(AAction *));
  void		addListener(const std::string &signal, void (AAction::*listener)());

protected:
  Data			&_data;
  Action::State		_state;
  std::queue<AAction *>	_actions;
  std::map<std::string, Action::Listeners>      _listeners;
private:
  uint32_t              _counter;
};

#endif
