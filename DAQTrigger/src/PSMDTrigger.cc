#include <cmath>

#include <yaml-cpp/yaml.h>

#include "PSMDTrigger.hh"

#include "AbsConfList.hh"
#include "ADCHeader.hh"
#include "FADCRawChannel.hh"
#include "FADCRawEvent.hh"
#include "IADCTConf.hh"
#include "adcconsts.hh"

PSMDTrigger::PSMDTrigger()
  : AbsSoftTrigger("PSMDTrigger")
{
}

void PSMDTrigger::DoConfig(AbsConfList * configs)
{
  fConfigs = configs;

  YAML::Node node = configs->GetYAMLNode("PSMDTrigger");
  if (node["ENABLED"] && node["ENABLED"].as<int>()) { SetEnable(); }
}

void PSMDTrigger::InitTrigger() {}

bool PSMDTrigger::DoTrigger(BuiltEvent * event)
{
  fTotalInputEvent++;

  bool triggered = false;

  int nadc = event->GetEntries();
  for (int i = 0; i < nadc; i++) {
    auto * raw = static_cast<AbsADCRaw *>(event->At(i));
    if (raw->GetADCType() != ADC::IADC) continue;

    auto * iadc = static_cast<FADCRawEvent *>(raw);
    ADCHeader * header = iadc->GetADCHeader();

    // Periodic/random (pedestal) trigger events carry no signal by design,
    // so psum gating would drop them. Keep them intact instead.
    if (header->GetTriggerType() == 0) {
      triggered = true;
      continue;
    }

    auto * conf = static_cast<IADCTConf *>(fConfigs->FindConfig(ADC::IADCT, header->GetMID()));
    if (!conf) continue;

    for (int g = 0; g < kNPSMDIADC; g++) {
      int psum = 0;
      for (int c = 0; c < kNCHPERPSMD; c++) {
        int ch = g * kNCHPERPSMD + c;
        int ped = static_cast<int>(header->GetPedestal(ch));
        unsigned short * adc = iadc->GetChannel(ch)->GetADC();

        int peak = 0;
        for (int j = 0; j < iadc->GetNDP(); j++) {
          int dev = std::abs(static_cast<int>(adc[j]) - ped);
          if (dev > peak) peak = dev;
        }
        psum += peak;
      }

      // Panels that don't clear their own threshold are dropped from the
      // output; the write path already skips channels marked suppressed.
      if (psum >= conf->PSUMTHR(g)) {
        triggered = true;
      }
      else {
        for (int c = 0; c < kNCHPERPSMD; c++)
          header->SetSuppressed(g * kNCHPERPSMD + c);
      }
    }
  }

  if (triggered) fNTriggeredEvent++;
  return triggered;
}
