#ifndef LEFTOVER_RECORD_H_
#define LEFTOVER_RECORD_H_
#include <iostream>
#include <string>
// Ivan Adame
// CPSC 121L-18
// April 17, 2025
// ivanadames@csu.fullerton.edu
// @Ivan-A0

class LeftoverRecord {
public:
  /*LeftoverRecord(const std::string &date, const std::string &meal,
                 const std::string &food_name, double qty_in_oz,
                 const std::string &leftover_reason,
                 const std::string &disposal_mechanism,
                 double cost)
  : date_(date),
    meal_(meal),
    food_name_(food_name),
    qty_in_oz_(qty_in_oz),
    leftover_reason_(leftover_reason),
    disposal_mechanism_(disposal_mechanism),
    cost_(cost) {}*/
  LeftoverRecord();

  void SetDate(const std::string &date) { date_ = date; }
  std::string GetDate() const { return date_; }

  void SetMeal(const std::string &meal) { meal_ = meal; }
  std::string GetMeal() const { return meal_; }

  void SetFoodName(const std::string &food_name) { food_name_ = food_name; }
  std::string GetFoodName() const { return food_name_; }

  void SetQuantityInOz(double qty_in_oz) { qty_in_oz_ = qty_in_oz; }
  double GetQuantityInOz() const { return qty_in_oz_; }

  void SetLeftoverReason(const std::string &leftover_reason) {
    leftover_reason_ = leftover_reason;
  }
  std::string GetLeftOverReason() const { return leftover_reason_; }

  void SetDisposalMechanism(const std::string &disposal_mechanism) {
    disposal_mechanism_ = disposal_mechanism;
  }
  std::string GetDisposalMechanism() const { return disposal_mechanism_; }

  void SetCost(double cost) { cost_ = cost; }
  double GetCost() const { return cost_; }

  bool operator==(const LeftoverRecord &other) const {
    if (date_ == other.date_ && meal_ == other.meal_ &&
        food_name_ == other.food_name_ && qty_in_oz_ == other.qty_in_oz_ &&
        leftover_reason_ == other.leftover_reason_ &&
        disposal_mechanism_ == other.disposal_mechanism_ &&
        cost_ == other.cost_) {
      return true;
    }
    return false;
  }

private:
  std::string date_;
  std::string meal_;
  std::string food_name_;
  double qty_in_oz_;
  std::string leftover_reason_;
  std::string disposal_mechanism_;
  double cost_;
};

#endif
