#pragma once

#include "AbsSoftTrigger.hh"

// Software trigger for IADC: computes the peak-sum (sum of per-channel peak
// deviations from pedestal) of each PSMD panel (4 consecutive IADC
// channels). Panels below their own threshold (IADCT YAML key "PSUMTHR",
// one value per panel) have their channels marked suppressed and dropped
// from output; the event itself is accepted if at least one panel clears
// its threshold.
//
// Events from the periodic/random (pedestal) trigger (ADCHeader
// GetTriggerType() == 0) carry no signal by design, so psum gating is
// skipped for them entirely -- they pass through untouched.
//
// On/off is controlled by the top-level "PSMDTrigger: { ENABLED: 1 }" YAML
// key; when disabled (default), DoTrigger is never called by
// CupDAQManager, so no suppression or event rejection happens at all.
class PSMDTrigger : public AbsSoftTrigger {
public:
  PSMDTrigger();
  ~PSMDTrigger() = default;

  void DoConfig(AbsConfList * configs) override;
  void InitTrigger() override;
  bool DoTrigger(BuiltEvent * event) override;

private:
  AbsConfList * fConfigs{nullptr};
};
