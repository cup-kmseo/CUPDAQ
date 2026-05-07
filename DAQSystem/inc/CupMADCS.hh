#pragma once

#include "AbsConf.hh"
#include "AbsADC.hh"
#include "NKFADC125S.hh"

class CupMADCS : public AbsADC {
public:
  CupMADCS() = default;
  CupMADCS(int sid);
  CupMADCS(AbsConf * config);
  ~CupMADCS() = default;

  int Open() override;
  void Close() override;

  bool Configure() override;
  bool Initialize() override;
  void StartTrigger() override;
  void StopTrigger() override;

  int ReadBCount() override;
  int ReadData(int count, unsigned char * data) override;
  int ReadData(int count) override;

private:
  void UpdateTriggerAndTime(const unsigned char * tempdata);

  NKFADC125S fFADC{};
};
