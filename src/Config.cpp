#include        "Config.hh"

Config          Config::_conf;

Config::Config()
{
  setNumber("refreshRate", 20);
  setNumber("clearOnResfresh", 1);
}

Config::~Config()
{
}
