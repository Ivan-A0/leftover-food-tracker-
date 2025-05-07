#ifndef LEFTOVER_TRACKER_H_
#define LEFTOVER_TRACKER_H_
#include <iostream>
#include <vector>

#include "leftover_record.h"
#include "leftover_report.h"
// Ivan Adame
// CPSC 121L-18
// April 17, 2025
// ivanadames@csu.fullerton.edu
// @Ivan-A0

class LeftoverTracker {
 public:
  /*LeftoverTracker(const std::vector<LeftoverRecord> &leftover_records)
      : leftover_records_(leftover_records) {} */
  LeftoverTracker();
  bool AddRecord(const LeftoverRecord &record) {
    for (const auto &existing_record : leftover_records_) {
      if (existing_record == record) {
        return false;
      }
    }
    leftover_records_.push_back(record);
    return true;
  }
  bool DeleteRecord(const LeftoverRecord &record) {
    for (auto it = leftover_records_.begin(); it != leftover_records_.end();
         it++) {
      if (it->GetDate() == record.GetDate() &&
          it->GetMeal() == record.GetMeal() &&
          it->GetFoodName() == record.GetFoodName() &&
          it->GetQuantityInOz() == record.GetQuantityInOz() &&
          it->GetLeftOverReason() == record.GetLeftOverReason() &&
          it->GetDisposalMechanism() == record.GetDisposalMechanism() &&
          it->GetCost() == record.GetCost()) {
        leftover_records_.erase(it);
        return true;
      }
    }
    return false;
  }
  const std::vector<LeftoverRecord> &GetRecords() const {
    return leftover_records_;
  }
  LeftoverReport GetLeftoverReport() const {
    return LeftoverReport(leftover_records_);
  }

 private:
  std::vector<LeftoverRecord> leftover_records_;
};

#endif
