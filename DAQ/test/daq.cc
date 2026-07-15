#include "TROOT.h"

#include "CupDAQManager.hh"
#include "daqopt.hh"

int main(int argc, char ** argv)
{
  if (argc < 2) {
    printusage(argv[0]);
    return 0;
  }

  ROOT::EnableThreadSafety();

  daqopt option;
  option.init();
  optparse(option, argc, argv);

  // for TCB controlled ADC
  ADC::TYPE adctype = static_cast<ADC::TYPE>(static_cast<int>(option.adctype[0]) + 10);

  auto * DAQ = new CupDAQManager();
  DAQ->SetDAQType(DAQ::TCBCTRL);
  DAQ->SetRunNumber(option.runnum);
  DAQ->SetADCType(adctype);
  DAQ->SetDAQID(option.daqid);
  DAQ->SetTriggerMode(TRIGGER::GLOBAL);
  DAQ->SetConfigFilename(option.config);
  DAQ->SetOutputFileFormat(option.format);
  DAQ->SetTriggerMonTime(option.rfreq);
  DAQ->SetVerboseLevel(option.vlevel);
  if (option.dosend) DAQ->UseEventMerger();
  if (option.dohist) DAQ->EnableHistograming();

  // No software trigger here: when UseEventMerger() is on (-x/dosend, the
  // normal case for a per-DAQID process feeding an IADCMERGER), BuildEvent
  // short-circuits past any registered SoftTrigger and forwards every event
  // as-is (see CupDAQManager_build.cc). Register software triggers in
  // merger.cc instead, where the fully merged event is actually judged.

  DAQ->Run();

  delete DAQ;

  return 0;
}
