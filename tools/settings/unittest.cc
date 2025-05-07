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

bool CopyJsonFile(const std::string& src, const std::string& dest) {
  std::string line;
  std::ifstream ini_file{src};  // This is the original file
  std::ofstream out_file{dest};
  if (ini_file && out_file) {
    while (getline(ini_file, line)) {
      out_file << line << "\n";
    }
  } else {
    return false;
  }
  // Closing file
  ini_file.close();
  out_file.close();
  return true;
}

TEST(LeftoverTrackerBackend, IntegrationTest) {
  const std::string kSrcFilename =
      "tools/settings/data/leftover_records.json";
  const std::string kTestFilename = "tools/settings/data/temp.json";
  CopyJsonFile(kSrcFilename, kTestFilename);
  {
    LeftoverTrackerBackend backend{kTestFilename};
    // 1. Load records from file
    ASSERT_TRUE(backend.LoadRecordsFromJSONFile())
        << "Failed to load records from JSON file.";
    json records = json::parse(backend.GetRecords().dump());
    if (records.at("num_records") > 6) {
      ASSERT_TRUE(records.at("num_records") == 6)
          << "Tracker contains more than expected records after loading from "
             "JSON.";
    } else {
      ASSERT_TRUE(records.at("num_records") == 6)
          << "Tracker contains fewer than expected records after loading from "
             "JSON.";
    }

    // 2. Get report
    json report = json::parse(backend.GetLeftoverReport().dump());

    json foodJson = report.at("most_common_leftover_");
    ASSERT_TRUE(jsonContains(foodJson, "Pasta"))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonContains(foodJson, "Cake"))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonContains(foodJson, "Milk"))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonContains(foodJson, "Oats"))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonContains(foodJson, "Rice"))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonContains(foodJson, "Bread"))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonListLength(foodJson) == 6)
        << "Expected exactly 6 most commonly wasted foods in this test case.";

    json reasonJson = report.at("most_common_leftover_reason_");
    ASSERT_TRUE(jsonContains(reasonJson, kTooMuchLeftOverT))
        << "Food Wastage report incorrectly computed wastage reasons.";
    ASSERT_TRUE(jsonContains(reasonJson, kExpiredT))
        << "Food Wastage report incorrectly computed wastage reasons.";
    ASSERT_TRUE(jsonContains(reasonJson, kTastesBadT))
        << "Food Wastage report incorrectly computed wastage reasons.";
    ASSERT_TRUE(jsonListLength(reasonJson) == 3)
        << "Expected exactly 1 most common food wastage reason in this test "
           "case.";

    json disposalMechanismJson = report.at("most_common_disposal_mechanism_");
    ASSERT_TRUE(jsonContains(disposalMechanismJson, kTrashT))
        << "Food Wastage report incorrectly computed most common disposal "
           "mechanism.";
    ASSERT_TRUE(jsonContains(disposalMechanismJson, kDonationT))
        << "Food Wastage report incorrectly computed most common disposal "
           "mechanism.";
    ASSERT_TRUE(jsonListLength(disposalMechanismJson) == 2)
        << "Expected exactly 2 disposal mechanisms in this test "
           "case.";
    ASSERT_TRUE(
        jsonContains(report.at("most_leftover_producing_meal_"), kDefaultMeal))
        << "Food Wastage report incorrectly computed most waste producing "
           "meal.";
    ASSERT_TRUE(jsonListLength(report.at("most_leftover_producing_meal_")) == 1)
        << "Expected exactly 1 most common waste producing meal in this test "
           "case.";
    ASSERT_TRUE(report.at("total_cost_of_leftover_") == 13.5)
        << "Food Wastage report incorrectly computed total cost of food "
           "wasted.";

    // Waste reduction strategies
    json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
    ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
        << "Food wastage report should suggest buying less food.";
    ASSERT_TRUE(jsonContains(strategiesJson, kCookSmallerServingsT))
        << "Food wastage report should suggest cooking smaller servings.";
    ASSERT_TRUE(jsonContains(strategiesJson, kDonateBeforeExpirationT))
        << "Food wastage report should suggest recycling lefotvers.";
    ASSERT_TRUE(jsonListLength(strategiesJson) == 3)
        << "Food wastage report should suggest 3 strategies";

    // 3. Add new record
    json add_status = json::parse(
        backend
            .AddRecord(QueryStringfromRecordDetails(
                "2023-02-12", kDefaultMeal, kDefaultFood, kDefaultQty,
                kDefaultDisposal, 5, kDefaultLeftoverReason))
            .dump());
    ASSERT_TRUE(add_status.at("success")) << "Adding record to tracker failed.";

    // 4. Get records
    records = json::parse(backend.GetRecords().dump());
    ASSERT_TRUE(records.at("num_records") == 7)
        << "Failed to return all the records.";

    // 5. Get report again
    report = json::parse(backend.GetLeftoverReport().dump());

    ASSERT_TRUE(
        jsonContains(report.at("most_common_leftover_"), kDefaultFood))
        << "Food Wastage report incorrectly computed most common food.";
    ASSERT_TRUE(jsonListLength(report.at("most_common_leftover_")) == 1)
        << "Expected exactly 1 most commonly wasted food in this test "
           "case.";
    ASSERT_TRUE(jsonContains(report.at("most_common_disposal_mechanism_"),
                             kDefaultDisposal))
        << "Food Wastage report incorrectly computed most common disposal "
           "mechanism mechanism.";
    ASSERT_TRUE(jsonListLength(report.at("most_common_disposal_mechanism_")) ==
                1)
        << "Expected exactly 1 most common disposal mechanism in this test "
           "case.";
    ASSERT_TRUE(jsonContains(report.at("most_common_leftover_reason_"),
                             kDefaultLeftoverReason))
        << "Food Wastage report incorrectly computed most common wastage "
           "reason.";
    ASSERT_TRUE(jsonListLength(report.at("most_common_leftover_reason_")) == 1)
        << "Expected exactly 1 most common wastage reason in this test "
           "case.";
    ASSERT_TRUE(
        jsonContains(report.at("most_leftover_producing_meal_"), kDefaultMeal))
        << "Food Wastage report incorrectly computed most waste producing "
           "meal.";
    ASSERT_TRUE(jsonListLength(report.at("most_leftover_producing_meal_")) == 1)
        << "Expected exactly 1 most common waste producing meal in this test "
           "case.";
    ASSERT_TRUE(report.at("total_cost_of_leftover_") == 18.5)
        << "Food Wastage report incorrectly computed total cost of food "
           "wasted.";

    // Waste reduction strategies
    strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
    ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
        << "Food wastage report should suggest buying less food.";
    ASSERT_TRUE(jsonContains(strategiesJson, kCookSmallerServingsT))
        << "Food wastage report should suggest cooking smaller servings.";
    ASSERT_TRUE(jsonContains(strategiesJson, kRecycleLeftOversT))
        << "Food wastage report should suggest recycling lefotvers.";
    ASSERT_TRUE(jsonListLength(strategiesJson) == 3)
        << "Food wastage report should suggest 3 strategies";

    // 6. Write record to JSON
    ASSERT_TRUE(backend.WriteRecordsToJSONFile())
        << "Tracker failed to write records to JSON.";
  }

  // Check if the records are actually persisted across multiple runs.
  {
    LeftoverTrackerBackend backend{kTestFilename};
    ASSERT_TRUE(backend.LoadRecordsFromJSONFile())
        << "Failed to load records from JSON file.";
    json records = json::parse(backend.GetRecords().dump());
    if (records.at("num_records") > 7) {
      ASSERT_TRUE(records.at("num_records") == 7)
          << "Tracker contains more than expected records after loading from "
             "JSON.";
    } else {
      ASSERT_TRUE(records.at("num_records") == 7)
          << "Tracker contains fewer than expected records after loading from "
             "JSON.";
    }
  }
  std::remove(kTestFilename.c_str());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}