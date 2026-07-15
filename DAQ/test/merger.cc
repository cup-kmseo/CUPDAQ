#include "TROOT.h"

#include "CupDAQManager.hh"
#include "QsumTrigger.hh"
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
  DAQ->SetDAQType(DAQ::MERGER);
  DAQ->SetRunNumber(option.runnum);
  DAQ->SetADCType(adctype);
  DAQ->SetDAQID(option.daqid);
  DAQ->SetConfigFilename(option.config);
  DAQ->SetOutputFileFormat(option.format);
  DAQ->SetTriggerMonTime(option.rfreq);
  DAQ->SetVerboseLevel(option.vlevel);
  if (option.dohist) DAQ->EnableHistograming();

  // Per-channel Qsum software trigger for IADC. No-op unless enabled via
  // YAML "QsumTrigger: { ENABLED: 1 }" and per-channel thresholds are set
  // via IADCT "QSUMTHR" (see QsumTrigger.hh). Registered here, not in
  // daq.cc, because per-DAQID processes forward to the merger with
  // UseEventMerger() and never call DoTrigger themselves; the merger sees
  // the fully merged event, which is what Qsum's accept/reject should
  // actually judge.
  auto * swtrigger = new QsumTrigger();
  swtrigger->SetDAQID(option.daqid);
  swtrigger->SetVerboseLevel(option.vlevel);
  DAQ->SetSoftTrigger(swtrigger);

  DAQ->Run();

  delete DAQ;

  return 0;
}
