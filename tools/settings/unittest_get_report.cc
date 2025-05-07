#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../../leftover_tracker_backend.h"
#include "../cppaudit/gtest_ext.h"
#include "test_utils.h"

using ::testing::HasSubstr;
using ::testing::Not;
using json = nlohmann::json;

TEST(LeftoverTrackerBackend, GetReportSingleRecord) {
  LeftoverTrackerBackend backend{""};
  backend.AddRecord(CreateDefaultQueryString());
  json report = json::parse(backend.GetLeftoverReport().dump());

  ASSERT_TRUE(
      jsonContains(report.at("most_common_leftover_"), kDefaultFood))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonListLength(report.at("most_common_leftover_")) == 1)
      << "Expected exactly 1 leftover food in this test "
         "case.";
  ASSERT_TRUE(jsonContains(report.at("most_common_disposal_mechanism_"),
                           kDefaultDisposal))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism mechanism.";
  ASSERT_TRUE(jsonListLength(report.at("most_common_disposal_mechanism_")) == 1)
      << "Expected exactly 1 most common disposal mechanism in this test "
         "case.";
  ASSERT_TRUE(jsonContains(report.at("most_common_leftover_reason_"),
                           kDefaultLeftoverReason))
      << "Leftover report incorrectly computed most common leftover reason.";
  ASSERT_TRUE(jsonListLength(report.at("most_common_leftover_reason_")) == 1)
      << "Expected exactly 1 most common leftover reason in this test "
         "case.";
  ASSERT_TRUE(
      jsonContains(report.at("most_leftover_producing_meal_"), kDefaultMeal))
      << "Leftover report incorrectly computed most leftover producing meal.";
  ASSERT_TRUE(jsonListLength(report.at("most_leftover_producing_meal_")) == 1)
      << "Expected exactly 1 most common leftover producing meal in this test "
         "case.";
  ASSERT_TRUE(report.at("total_cost_of_leftover_") == kDefaultCost)
      << "Leftover report incorrectly computed total cost of leftover.";

  // Leftover reduction strategies
  json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
      << "Leftover report should suggest buying less food.";
  ASSERT_TRUE(jsonContains(strategiesJson, kCookSmallerServingsT))
      << "Leftover report should suggest cooking smaller servings.";
  ASSERT_TRUE(jsonContains(strategiesJson, kRecycleLeftOversT))
      << "Leftover report should suggest recycling lefotvers.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 3)
      << "Leftover report should suggest 3 strategies";
}

TEST(LeftoverTrackerBackend, GetReportMultipleRecords) {
  // Load records to backend
  LeftoverTrackerBackend backend{""};
  ASSERT_TRUE(loadTrackerWithDefaultTestData(backend));
  json records = json::parse(backend.GetRecords().dump());
  ASSERT_TRUE(records.at("num_records") == 6)
      << "Initializing tracker with test data failed.";

  json report = json::parse(backend.GetLeftoverReport().dump());

  json foodJson = report.at("most_common_leftover_");
  ASSERT_TRUE(jsonContains(foodJson, "Pasta"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonContains(foodJson, "Cake"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonContains(foodJson, "Milk"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonContains(foodJson, "Oats"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonContains(foodJson, "Rice"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonContains(foodJson, "Bread"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonListLength(foodJson) == 6)
      << "Expected exactly 6 most commonly leftover foods in this test case.";

  json reasonJson = report.at("most_common_leftover_reason_");
  ASSERT_TRUE(jsonContains(reasonJson, kTooMuchLeftOverT))
      << "Leftover report incorrectly computed leftover reasons.";
  ASSERT_TRUE(jsonContains(reasonJson, kExpiredT))
      << "Leftover report incorrectly computed leftover reasons.";
  ASSERT_TRUE(jsonContains(reasonJson, kTastesBadT))
      << "Leftover report incorrectly computed leftover reasons.";
  ASSERT_TRUE(jsonListLength(reasonJson) == 3)
      << "Expected exactly 1 most common leftover reason in this test "
         "case.";

  json disposalMechanismJson = report.at("most_common_disposal_mechanism_");
  ASSERT_TRUE(jsonContains(disposalMechanismJson, kTrashT))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonContains(disposalMechanismJson, kDonationT))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonListLength(disposalMechanismJson) == 2)
      << "Expected exactly 2 disposal mechanisms in this test "
         "case.";

  ASSERT_TRUE(report.at("most_leftover_producing_meal_")[0] == kDefaultMeal)
      << "Leftover report incorrectly computed most leftover producing "
         "meal.";
  ASSERT_TRUE(jsonListLength(report.at("most_leftover_producing_meal_")) == 1)
      << "Expected exactly 1 most common leftover producing meal in this test "
         "case.";

  ASSERT_TRUE(report.at("total_cost_of_leftover_") == 13.5)
      << "Leftover report incorrectly computed total cost of food "
         "leftover.";

  // Leftover reduction strategies
  json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
      << "Leftover report should suggest buying less food.";
  ASSERT_TRUE(jsonContains(strategiesJson, kCookSmallerServingsT))
      << "Leftover report should suggest cooking smaller servings.";
  ASSERT_TRUE(jsonContains(strategiesJson, kDonateBeforeExpirationT))
      << "Leftover report should suggest recycling lefotvers.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 3)
      << "Leftover report should suggest 3 strategies";
}

// Get empty report
TEST(LeftoverTrackerBackend, GetReportEmpty) {
  LeftoverTrackerBackend backend{""};

  json report = json::parse(backend.GetLeftoverReport().dump());

  ASSERT_TRUE(jsonEmpty(report.at("most_common_leftover_")))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonEmpty(report.at("most_common_disposal_mechanism_")))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonEmpty(report.at("most_common_leftover_reason_")))
      << "Leftover report incorrectly computed most common leftover reason.";
  ASSERT_TRUE(jsonEmpty(report.at("most_leftover_producing_meal_")))
      << "Leftover report incorrectly computed most eftover producing meal.";
  ASSERT_TRUE(jsonEmpty(report.at("suggested_strategies_to_reduce_leftover_")));
  ASSERT_TRUE(report.at("total_cost_of_leftover_") == 0)
      << "Leftover report incorrectly computed total cost of food leftover.";
}

// Get report leftover reduction logic correctness
TEST(LeftoverTrackerBackend, GetReportLeftoverReductionTestTooMuchLeftovers) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status =
      json::parse(backend
                      .AddRecord(CreateDefaultQueryStringWithLeftoverReason(
                          kTooMuchLeftOverT))
                      .dump());
  ASSERT_TRUE(add_status.at("success")) << "Failed to add record to tracker.";

  json report = json::parse(backend.GetLeftoverReport().dump());

  // Leftover reduction strategies
  json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
      << "Leftover report should suggest buying less food.";
  ASSERT_TRUE(jsonContains(strategiesJson, kCookSmallerServingsT))
      << "Leftover report should suggest cooking smaller servings.";
  ASSERT_TRUE(jsonContains(strategiesJson, kRecycleLeftOversT))
      << "Leftover report should suggest recycling lefotvers.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 3)
      << "Leftover report should suggest 3 strategies";
}

// Get report leftover reduction logic correctness
TEST(LeftoverTrackerBackend, GetReportLeftoeverReductionTestTastesBad) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithLeftoverReason(kTastesBadT))
          .dump());
  ASSERT_TRUE(add_status.at("success")) << "Adding record to tracker failed.";

  json report = json::parse(backend.GetLeftoverReport().dump());
  // Leftover reduction strategies
  json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
      << "Leftover report should suggest buying less food.";
  ASSERT_TRUE(jsonContains(strategiesJson, kRecycleLeftOversT))
      << "Leftover report should suggest recycling lefotvers.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 2)
      << "Leftover report should suggest 2 strategies";
}

// Get report leftover reduction logic correctness
TEST(LeftoverTrackerBackend, GetReportLeftoverReductionTestExpired) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithLeftoverReason(kExpiredT))
          .dump());
  ASSERT_TRUE(add_status.at("success")) << "Adding record to tracker failed.";

  json report = json::parse(backend.GetLeftoverReport().dump());

  // Leftover reduction strategies
  json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kDonateBeforeExpirationT))
      << "Leftover report should suggest donating before expiration.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 1)
      << "Leftover report should suggest 1 strategy";
}

// Get report most commonly leftover food
TEST(LeftoverTrackerBackend, GetReportMostCommonLeftoverFood) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithFoodName("Pasta")).dump());
  ASSERT_TRUE(add_status.at("success")) << "Adding record to tracker failed.";

  json report = json::parse(backend.GetLeftoverReport().dump());
  ASSERT_TRUE(report.at("most_common_leftover_")[0] == "Pasta")
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonListLength(report.at("most_common_leftover_")) == 1)
      << "Expected exactly one most commonly leftover food.";

  // Add second record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithFoodName("Chips")).dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Adding second record to tracker failed.";

  report = json::parse(backend.GetLeftoverReport().dump());
  json foodJson = report.at("most_common_leftover_");
  ASSERT_TRUE(jsonContains(foodJson, "Pasta"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonContains(foodJson, "Chips"))
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonListLength(foodJson) == 2)
      << "Expected exactly 2 most common leftover foods in this test case.";

  // Add new record
  add_status =
      json::parse(backend
                      .AddRecord(QueryStringfromRecordDetails(
                          kDefaultDate, kDefaultMeal, "Pasta", kDefaultQty,
                          kDefaultDisposal, 9, kDefaultLeftoverReason))
                      .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Adding third record to tracker failed.";

  report = json::parse(backend.GetLeftoverReport().dump());
  ASSERT_TRUE(report.at("most_common_leftover_")[0] == "Pasta")
      << "Leftover report incorrectly computed most common food.";
  ASSERT_TRUE(jsonListLength(report.at("most_common_leftover_")) == 1)
      << "Expected exactly 1 leftover food in this test case.";
}

// Get report ComputeMostLeftoverProducingMeal
TEST(LeftoverTrackerBackend, GetReportMostLeftoverProducingMeal) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithMeal("Dinner")).dump());
  ASSERT_TRUE(add_status.at("success")) << "Failed to add record to tracker.";

  json report = json::parse(backend.GetLeftoverReport().dump());
  ASSERT_TRUE(report.at("most_leftover_producing_meal_")[0] == "Dinner")
      << "Leftover report incorrectly computed most leftover producing meal.";
  ASSERT_TRUE(jsonListLength(report.at("most_leftover_producing_meal_")) == 1)
      << "Expected exactly 1 most leftover producing meal in this test case.";

  // Add second record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithMeal("Breakfast")).dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add second record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  json foodJson = report.at("most_leftover_producing_meal_");
  ASSERT_TRUE(jsonContains(foodJson, "Dinner"))
      << "Leftover report incorrectly computed most leftover producing meal.";
  ASSERT_TRUE(jsonContains(foodJson, "Breakfast"))
      << "Leftover report incorrectly computed most leftover producing meal.";
  ASSERT_TRUE(jsonListLength(foodJson) == 2)
      << "Expected exactly 2 most leftover producing meals in this test case.";

  // Add new record
  add_status =
      json::parse(backend
                      .AddRecord(QueryStringfromRecordDetails(
                          kDefaultDate, "Dinner", kDefaultFood, kDefaultQty,
                          kDefaultDisposal, 9, kDefaultLeftoverReason))
                      .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add third record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  foodJson = report.at("most_leftover_producing_meal_");
  ASSERT_TRUE(jsonContains(foodJson, "Dinner"))
      << "Leftover report incorrectly computed most leftover producing meal.";
  ASSERT_TRUE(jsonListLength(foodJson) == 1)
      << "Expected exactly 1 most leftover producing meal in this test case.";
}

// Get report ComputeTotalCostOfLeftover
TEST(LeftoverTrackerBackend, GetReportComputeTotalCostOfFoodLeftover) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithMeal("Dinner")).dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add first record to tracker.";

  json report = json::parse(backend.GetLeftoverReport().dump());
  ASSERT_TRUE(report.at("total_cost_of_leftover_") == 4.5)
      << "Leftover report incorrectly computed total cost of leftovers.";

  // Add second record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithMeal("Breakfast")).dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add second record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  ASSERT_TRUE(report.at("total_cost_of_leftover_") == 9)
      << "Leftover report incorrectly computed total cost of leftover.";

  // Add new record
  add_status =
      json::parse(backend
                      .AddRecord(QueryStringfromRecordDetails(
                          kDefaultDate, "Dinner", kDefaultFood, kDefaultQty,
                          kDefaultDisposal, 9, kDefaultLeftoverReason))
                      .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add third record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  ASSERT_TRUE(report.at("total_cost_of_leftover_") == 18)
      << "Leftover report incorrectly computed total cost of leftovers.";
}

// ComputeMostCommonLeftoverReason
TEST(LeftoverTrackerBackend, GetReportComputeMostCommonLeftOverReason) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status =
      json::parse(backend
                      .AddRecord(CreateDefaultQueryStringWithLeftoverReason(
                          kTooMuchLeftOverT))
                      .dump());
  ASSERT_TRUE(add_status.at("success")) << "Failed to add record to tracker.";

  json report = json::parse(backend.GetLeftoverReport().dump());
  json foodJson = report.at("most_common_leftover_reason_");
  ASSERT_TRUE(jsonContains(foodJson, kTooMuchLeftOverT))
      << "Leftover report incorrectly computed leftover reasons.";
  ASSERT_TRUE(jsonListLength(foodJson) == 1)
      << "Expected exactly 1 most common leftover reason in this test "
         "case.";

  ASSERT_TRUE(report.at("total_cost_of_leftover_") == 4.5)
      << "Leftover report incorrectly computed total cost of leftovers.";

  // Add second record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithLeftoverReason(kTastesBadT))
          .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add second record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  foodJson = report.at("most_common_leftover_reason_");
  ASSERT_TRUE(jsonContains(foodJson, kTooMuchLeftOverT))
      << "Leftover report incorrectly computed leftover reasons.";
  ASSERT_TRUE(jsonContains(foodJson, kTastesBadT))
      << "Leftover report incorrectly computed leftover reasons.";
  ASSERT_TRUE(jsonListLength(foodJson) == 2)
      << "Expected exactly 2 most common leftover reasons in this test "
         "case.";

  // Add new record
  add_status =
      json::parse(backend
                      .AddRecord(QueryStringfromRecordDetails(
                          kDefaultDate, kDefaultMeal, kDefaultFood, kDefaultQty,
                          kDefaultDisposal, 9, kTooMuchLeftOverT))
                      .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add third record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  foodJson = report.at("most_common_leftover_reason_");
  ASSERT_TRUE(jsonContains(foodJson, kTooMuchLeftOverT))
      << "Leftover report incorrectly computed leftover reasons.";
  ;
  ASSERT_TRUE(jsonListLength(foodJson) == 1)
      << "Expected exactly 1 most common food leftover reason in this test "
         "case.";
}

// ComputeMostCommonDisposalMechanism
TEST(LeftoverTrackerBackend, GetReportComputeMostCommonDisposalMechanism) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithDisposalMechanism(kSinkT))
          .dump());
  ASSERT_TRUE(add_status.at("success")) << "Failed to add record to tracker.";

  json report = json::parse(backend.GetLeftoverReport().dump());
  json foodJson = report.at("most_common_disposal_mechanism_");
  ASSERT_TRUE(jsonContains(foodJson, kSinkT))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonListLength(foodJson) == 1)
      << "Expected exactly 1 most common disposal mechanism in this test case.";

  // Add second record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithDisposalMechanism(kTrashT))
          .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add second record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  foodJson = report.at("most_common_disposal_mechanism_");
  ASSERT_TRUE(jsonContains(foodJson, kTrashT))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonContains(foodJson, kSinkT))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonListLength(foodJson) == 2)
      << "Expected exactly 2 most common disposal mechanisms in this test "
         "case.";

  // Add new record
  add_status =
      json::parse(backend
                      .AddRecord(QueryStringfromRecordDetails(
                          kDefaultDate, kDefaultMeal, kDefaultFood, kDefaultQty,
                          kTrashT, 9, kDefaultLeftoverReason))
                      .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add third record to tracker.";

  report = json::parse(backend.GetLeftoverReport().dump());
  foodJson = report.at("most_common_disposal_mechanism_");
  ASSERT_TRUE(jsonContains(foodJson, kTrashT))
      << "Leftover report incorrectly computed most common disposal "
         "mechanism.";
  ASSERT_TRUE(jsonListLength(foodJson) == 1)
      << "Expected exactly 1 most common disposal mechanism in this test case.";
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}
