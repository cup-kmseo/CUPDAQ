#include <yaml-cpp/yaml.h>

#include "QsumTrigger.hh"

#include "AbsConfList.hh"
#include "ADCHeader.hh"
#include "FADCRawChannel.hh"
#include "FADCRawEvent.hh"
#include "IADCTConf.hh"
#include "adcconsts.hh"

QsumTrigger::QsumTrigger()
  : AbsSoftTrigger("QsumTrigger")
{
}

void QsumTrigger::DoConfig(AbsConfList * configs)
{
  fConfigs = configs;

  YAML::Node node = configs->GetYAMLNode("QsumTrigger");
  if (node["ENABLED"] && node["ENABLED"].as<int>()) { SetEnable(); }
}

void QsumTrigger::InitTrigger() {}

bool QsumTrigger::DoTrigger(BuiltEvent * event)
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
    // so Qsum gating would drop them. Keep them intact instead.
    if (header->GetTriggerType() == 1) {
      triggered = true;
      continue;
    }

    auto * conf = static_cast<IADCTConf *>(fConfigs->FindConfig(ADC::IADCT, header->GetMID()));
    if (!conf) continue;

    for (int ch = 0; ch < kNCHIADC; ch++) {
      if (header->GetZero(ch) || header->GetSuppressed(ch)) continue;

      int ped = static_cast<int>(header->GetPedestal(ch));
      unsigned short * adc = iadc->GetChannel(ch)->GetADC();

      int qsum = 0;
      for (int j = 0; j < iadc->GetNDP(); j++) {
        qsum += static_cast<int>(adc[j]) - ped;
      }

      // Channels that don't clear their own threshold are dropped from the
      // output; the write path already skips channels marked suppressed.
      if (qsum >= conf->QSUMTHR(ch)) {
        triggered = true;
      }
      else {
        header->SetSuppressed(ch);
      }
    }
  }

  if (triggered) fNTriggeredEvent++;
  return triggered;
}
