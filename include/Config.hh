#ifndef         __CONFIG_HH__
#define         __CONFIG_HH__

#include        <map>
#include	<stdint.h>

class           Config
{
public:
  Config();
  ~Config();

public:
  static const std::string      &getString(const std::string &key) { return (_conf._strMap[key]); }
  static void                   setString(const std::string &key, const std::string &val) { _conf._strMap[key] = val; }
  static int                    getNumber(const std::string &key) { return (_conf._intMap[key]); }
  static void                   setNumber(const std::string &key, int val) { _conf._intMap[key] = val; }

private:
  std::map<std::string, int>            _intMap;
  std::map<std::string, std::string>    _strMap;
  static Config _conf;
};

#endif
