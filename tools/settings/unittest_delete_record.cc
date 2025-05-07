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

// Basic delete record
TEST(LeftoverTrackerBackend, DeleteRecords) {
  LeftoverTrackerBackend backend{""};
  backend.AddRecord(CreateDefaultQueryString());
  json delete_status =
      json::parse(backend.DeleteRecord(CreateDefaultQueryString()).dump());
  ASSERT_TRUE(delete_status.at("success"))
      << "Failed to delete the food wastage record.";
}

// Delete record when tracker is empty
TEST(LeftoverTrackerBackend, DeleteRecordFromEmptyTracker) {
  LeftoverTrackerBackend backend{""};
  json delete_status =
      json::parse(backend.DeleteRecord(CreateDefaultQueryString()).dump());
  ASSERT_FALSE(delete_status.at("success"))
      << "Deleting nonexistent record should not succeed";
}

// Delete a record that doesn't exist in the tracker
TEST(LeftoverTrackerBackend, DeleteNonexistentRecordFromTracker) {
  LeftoverTrackerBackend backend{""};

  backend.AddRecord(CreateDefaultQueryString());

  json delete_status = json::parse(
      backend.DeleteRecord(CreateDefaultQueryStringWithFoodName("Pizza"))
          .dump());
  ASSERT_FALSE(delete_status.at("success"))
      << "Deleting nonexistent record should not succeed";
}

// Try deleting the same record twice
TEST(LeftoverTrackerBackend, AccidentalDoubleDeletion) {
  LeftoverTrackerBackend backend{""};

  backend.AddRecord(CreateDefaultQueryString());

  json delete_status =
      json::parse(backend.DeleteRecord(CreateDefaultQueryString()).dump());
  ASSERT_TRUE(delete_status.at("success"))
      << "Failed to delete the food wastage record.";

  delete_status =
      json::parse(backend.DeleteRecord(CreateDefaultQueryString()).dump());
  ASSERT_FALSE(delete_status.at("success"))
      << "Deleting the same record twice should fail on the second attempt.";
}

// Add, then delete, then get a record (Error)
TEST(LeftoverTrackerBackend, AddDeleteGetRecord) {
  // Load records to backend
  LeftoverTrackerBackend backend{""};
  ASSERT_TRUE(loadTrackerWithDefaultTestData(backend));
  json records = json::parse(backend.GetRecords().dump());
  ASSERT_TRUE(records.at("num_records") == 6)
      << "Initializing tracker with test data failed.";

  // Create and add new record
  json add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithFoodName("Pineapples"))
          .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add the food wastage record.";

  // There are now 7 records
  records = json::parse(backend.GetRecords().dump());
  ASSERT_TRUE(records.at("num_records") == 7)
      << "Number of records should be higher after an add.";

  // Delete the added record
  json delete_status = json::parse(
      backend.DeleteRecord(CreateDefaultQueryStringWithFoodName("Pineapples"))
          .dump());
  ASSERT_TRUE(delete_status.at("success"))
      << "Failed to delete the food wastage record.";

  // There are now 6 records
  records = json::parse(backend.GetRecords().dump());
  ASSERT_TRUE(records.at("num_records") == 6)
      << "Number of records should be lowered after a delete.";
}

// Get report waste reduction logic correctness
TEST(LeftoverTrackerBackend, GetReportWasteReductionWithDeleteTest) {
  LeftoverTrackerBackend backend{""};

  // Add new record
  json add_status =
      json::parse(backend
                      .AddRecord(CreateDefaultQueryStringWithLeftoverReason(
                          kTooMuchLeftOverT))
                      .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add the food wastage record.";

  json report = json::parse(backend.GetLeftoverReport().dump());

  // Waste reduction strategies
  json strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
      << "Food wastage report should suggest buying less food.";
  ASSERT_TRUE(jsonContains(strategiesJson, kCookSmallerServingsT))
      << "Food wastage report should suggest cooking smaller servings.";
  ASSERT_TRUE(jsonContains(strategiesJson, kRecycleLeftOversT))
      << "Food wastage report should suggest recycling lefotvers.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 3)
      << "Food wastage report should suggest 3 strategies";

  // Delete record
  json delete_status =
      json::parse(backend
                      .DeleteRecord(CreateDefaultQueryStringWithLeftoverReason(
                          kTooMuchLeftOverT))
                      .dump());
  ASSERT_TRUE(delete_status.at("success"))
      << "Failed to delete the food wastage record.";

  // Add new record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithLeftoverReason(kTastesBadT))
          .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add the second food wastage record.";
  ;

  report = json::parse(backend.GetLeftoverReport().dump());

  // Waste reduction strategies
  strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kBuyLessFoodT))
      << "Food wastage report should suggest buying less food.";
  ASSERT_TRUE(jsonContains(strategiesJson, kRecycleLeftOversT))
      << "Food wastage report should suggest recycling lefotvers.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 2)
      << "Food wastage report should suggest 2 strategies";

  // Delete record
  delete_status = json::parse(
      backend
          .DeleteRecord(CreateDefaultQueryStringWithLeftoverReason(kTastesBadT))
          .dump());
  ASSERT_TRUE(delete_status.at("success"))
      << "Failed to delete the second food wastage record.";

  // Add new record
  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithLeftoverReason(kExpiredT))
          .dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add the third food wastage record.";

  report = json::parse(backend.GetLeftoverReport().dump());

  // Waste reduction strategies
  strategiesJson = report.at("suggested_strategies_to_reduce_leftover_");
  ASSERT_TRUE(jsonContains(strategiesJson, kDonateBeforeExpirationT))
      << "Food wastage report should suggest donating before expiration.";
  ASSERT_TRUE(jsonListLength(strategiesJson) == 1)
      << "Food wastage report should suggest 1 strategy";

  // Delete record
  delete_status = json::parse(
      backend.DeleteRecord(CreateDefaultQueryStringWithLeftoverReason(kExpiredT))
          .dump());
  ASSERT_TRUE(delete_status.at("success"))
      << "Failed to delete the third food wastage record.";
}

// Add then delete tons of records, calling Get along the way to ensure no
// inconsistencies pop up
TEST(LeftoverTrackerBackend, AddAndDeleteManyRecords) {
  LeftoverTrackerBackend backend{""};
  for (int i = 0; i < BIG_N; i++) {
    json add_status = json::parse(
        backend.AddRecord(CreateDefaultQueryStringWithCost(i + 1)).dump());
    ASSERT_TRUE(add_status.at("success"))
        << "Failed to add food wastage record number " << i + 1 << ".";

    json records = json::parse(backend.GetRecords().dump());
    ASSERT_TRUE(records.at("num_records") == i + 1)
        << "Adding food wastage record number " << i + 1
        << " did not update the number of records in the tracker.";
  }

  for (int i = 0; i < BIG_N; i++) {
    json delete_status = json::parse(
        backend.DeleteRecord(CreateDefaultQueryStringWithCost(i + 1)).dump());
    ASSERT_TRUE(delete_status.at("success"))
        << "Failed to delete food wastage record number " << i + 1 << ".";

    json records = json::parse(backend.GetRecords().dump());
    ASSERT_TRUE(records.at("num_records") == BIG_N - (i + 1))
        << "Adding food wastage record number " << (BIG_N - (i + 1))
        << " did not update the number of records in the tracker.";
  }

  json records = json::parse(backend.GetRecords().dump());
  ASSERT_TRUE(records.at("num_records") == 0)
      << "Tracker should be empty at the end of this test.";
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}
