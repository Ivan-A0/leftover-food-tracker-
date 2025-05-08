#ifndef LEFTOVER_REPORT_H_
#define LEFTOVER_REPORT_H_
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "leftover_record.h"
// Ivan Adame
// CPSC 121L-18
// April 17, 2025
// ivanadames@csu.fullerton.edu
// @Ivan-A0

class LeftoverReport {
public:
  LeftoverReport() {}
  LeftoverReport(const std::vector<LeftoverRecord> &leftover_records)
      : leftover_records_(leftover_records) {}

  std::vector<std::string> MostCommonLeftover() const {
    std::map<std::string, int> leftovers;
    std::vector<std::string> most_common_leftover_;
    int leftover_freq = 0;
    for (LeftoverRecord record : leftover_records_) {
      std::string food_name = record.GetFoodName();
      if (leftovers.find(food_name) == leftovers.end()) {
        leftovers[food_name] += 1;
      } else {
        leftovers[food_name] += 1;
      }
      if (leftovers[food_name] > leftover_freq) {
        leftover_freq = leftovers[food_name];
      }
    }
    for (std::pair<std::string, int> pair : leftovers) {
      if (pair.second == leftover_freq) {
        most_common_leftover_.push_back(pair.first);
      }
    }
    return most_common_leftover_;
  }

  std::vector<std::string> MostCostlyLeftoverProducingMeals() const {
    std::map<std::string, double> costly_meals;
    std::vector<std::string> most_costly_leftovers;
    double most_cost = 0.0;
    for (LeftoverRecord record : leftover_records_) {
      std::string meal = record.GetMeal();
      if (costly_meals.find(meal) == costly_meals.end()) {
        costly_meals[meal] = record.GetCost();
      } else {
        if (record.GetCost() > costly_meals[meal]) {
          costly_meals[meal] = record.GetCost();
        }
      }
      if (costly_meals[meal] > most_cost) {
        most_cost = costly_meals[meal];
      }
    }
    for (std::pair<std::string, double> pair : costly_meals) {
      if (pair.second == most_cost) {
        most_costly_leftovers.push_back(pair.first);
      }
    }
    return most_costly_leftovers;
  }

  double TotalCostOfLeftover() const {
    double tot_cost = 0.0;
    for (const auto &record : leftover_records_) {
      tot_cost += record.GetCost();
    }
    return tot_cost;
  }

  std::vector<std::string> MostCommonLeftoverReason() const {
    std::map<std::string, int> common_leftovers;
    std::vector<std::string> most_common_leftover_reason_;
    int most_common = 0;

    for (const LeftoverRecord &record : leftover_records_) {
      std::string reason = record.GetLeftOverReason();
      common_leftovers[reason]++;
      if (common_leftovers[reason] > most_common) {
        most_common = common_leftovers[reason];
      }
    }
    for (const auto &pair : common_leftovers) {
      if (pair.second == most_common) {
        most_common_leftover_reason_.push_back(pair.first);
      }
    }

    return most_common_leftover_reason_;
  }

  std::vector<std::string> MostCommonDisposalMechanism() const {
    std::map<std::string, int> most_common_disposal_;
    std::vector<std::string> most_common_disposal_mechanisms_;
    int common_disposal = 0;

    for (LeftoverRecord record : leftover_records_) {
      std::string mechanisms = record.GetDisposalMechanism();
      if (most_common_disposal_.find(mechanisms) ==
          most_common_disposal_.end()) {
        most_common_disposal_[mechanisms] = 1;
      } else {
        most_common_disposal_[mechanisms] += 1;
      }
      if (most_common_disposal_[mechanisms] > common_disposal) {
        common_disposal = most_common_disposal_[mechanisms];
      }
    }
    for (std::pair<std::string, int> pair : most_common_disposal_) {
      if (pair.second == common_disposal) {
        most_common_disposal_mechanisms_.push_back(pair.first);
      }
    }
    return most_common_disposal_mechanisms_;
  }
  std::vector<std::string> SuggestedLeftoverReductionStrategies() const {
    std::vector<std::string> strategies;
    std::vector<std::string> common_reasons = MostCommonLeftoverReason();

    if (common_reasons.empty()) {
      return strategies;
    }
    for (const std::string &reason : common_reasons) {
      if (reason == "Expired") {
        if (std::find(strategies.begin(), strategies.end(),
                      "Donate before expiration") == strategies.end()) {
          strategies.push_back("Donate before expiration");
        }
      }
      if (reason == "Tastes bad") {
        if (std::find(strategies.begin(), strategies.end(), "Buy less food") ==
            strategies.end()) {
          strategies.push_back("Buy less food");
        }
      }
      if (reason == "Too much left overs") {
        if (std::find(strategies.begin(), strategies.end(), "Buy less food") ==
            strategies.end()) {
          strategies.push_back("Buy less food");
        }
        if (std::find(strategies.begin(), strategies.end(),
                      "Cook small servings") == strategies.end()) {
          strategies.push_back("Cook small servings");
        }
      }
    }
    if (std::find(strategies.begin(), strategies.end(),
                  "Donate before expiration") == strategies.end()) {
      strategies.push_back("Recycle left overs");
    }
    return strategies;
  }

private:
  const std::vector<LeftoverRecord> leftover_records_;
  std::vector<std::string> most_common_leftover_;
  std::vector<std::string> most_costly_leftover_producing_meals_;
  double total_cost_of_leftover_;
  std::vector<std::string> most_common_leftover_reason_;
  std::vector<std::string> most_common_disposal_mechanism_;
  std::vector<std::string> strategies_;
};

#endif
