#pragma once

#include "DAQConfig/AbsConfList.hh"
#include "DAQConfig/FADCTConf.hh"
#include "DAQConfig/IADCTConf.hh"
#include "DAQConfig/SADCTConf.hh"
#include "DAQConfig/TCBConf.hh"
#include "DAQSystem/AbsTCB.hh"
#include "OnlConsts/onlconsts.hh"

class CupGeneralTCB {
public:
  CupGeneralTCB();
  ~CupGeneralTCB() = default;

  int Open();
  void Close();

  bool Config();
  bool StartTrigger();
  void StopTrigger();

  void SetConfig(AbsConfList * configs);
  void SetIPAddress(const char * ipaddr);
  void SetADCType(ADC::TYPE type);

private:
  int CheckLinkStatus();
  bool ConfigTCB(TCBConf * conf);
  bool ConfigFADC(FADCTConf * conf);
  bool ConfigSADC(SADCTConf * conf);
  bool ConfigIADC(IADCTConf * conf);

private:
  AbsTCB * fTCB;
  TCBConf * fTCBConfig;
  AbsConfList * fConfigs;

  TCB::TYPE fTCBType;
  ADC::TYPE fADCType;
  DAQ::EXPERIMENT fExperiment;
};

inline void CupGeneralTCB::SetConfig(AbsConfList * configs) { fConfigs = configs; }

inline void CupGeneralTCB::SetADCType(ADC::TYPE type) { fADCType = type; }
