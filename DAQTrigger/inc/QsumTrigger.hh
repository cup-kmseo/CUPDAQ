#pragma once

#include "AbsSoftTrigger.hh"

// Software trigger for IADC: per channel, sums the pedestal-subtracted
// sample values (signed, no abs()) over the waveform -- the same "Qsum"
// definition used by the old CUPDAQ CupSoftTrigger. Channels whose Qsum
// doesn't clear their own threshold (IADCT YAML key "QSUMTHR", one value
// per channel) are marked suppressed and dropped from output; the event
// itself is accepted if at least one channel clears its threshold.
//
// Events from the periodic/random (pedestal) trigger (ADCHeader
// GetTriggerType() == 1) carry no signal by design, so Qsum gating is
// skipped for them entirely -- they pass through untouched.
//
// On/off is controlled by the top-level "QsumTrigger: { ENABLED: 1 }" YAML
// key; when disabled (default), DoTrigger is never called by
// CupDAQManager, so no suppression or event rejection happens at all.
class QsumTrigger : public AbsSoftTrigger {
public:
  QsumTrigger();
  ~QsumTrigger() = default;

  void DoConfig(AbsConfList * configs) override;
  void InitTrigger() override;
  bool DoTrigger(BuiltEvent * event) override;

private:
  AbsConfList * fConfigs{nullptr};
};
