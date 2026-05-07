#pragma once

#include "FADCSConf.hh"

class GADCSConf : public FADCSConf {
public:
  GADCSConf();
  explicit GADCSConf(int sid);
  ~GADCSConf() override = default;

  void PrintConf() const override;

  ClassDef(GADCSConf, 1)
};