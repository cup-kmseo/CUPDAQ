#include <yaml-cpp/yaml.h>

#include "WCMDTrigger.hh"

WCMDTrigger::WCMDTrigger()
  : AbsSoftTrigger("WCMDTrigger")
{
}

void WCMDTrigger::DoConfig(AbsConfList * configs)
{
  YAML::Node node = configs->GetYAMLNode("WCMDTrigger");
}

void WCMDTrigger::InitTrigger()
{
}

bool WCMDTrigger::DoTrigger(BuiltEvent * event)
{
  fTotalInputEvent++;
  fNTriggeredEvent++;
  return true;
}
