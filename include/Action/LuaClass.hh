#ifndef         __LUACLASS_HH__
#define         __LUACLASS_HH__

#include        <iostream>
#include        <LuaBridge.h>
#include	"Data.hh"
#include	"AAction.hh"
#include	"PokemonUtils.hh"
#include	"../vbam/gba/Globals.h"
#include	"../vbam/sdl/SDLGlobals.h"

using namespace luabridge;

namespace       Action
{
  class         LuaClass : public AAction
  {
  public:
    static LuaClass     *create(LuaRef ref) { return (new LuaClass(ref)); }

  public:
    LuaClass(LuaRef ref);
    ~LuaClass();

  public:
    void        _init();
    void        _update();

  private:
    LuaRef      _ref;
    LuaRef      _initFunc;
    LuaRef      _updateFunc;
  };
};

#endif
