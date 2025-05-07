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

TEST(LeftoverTrackerBackend, AddDefaultRecord) {
  LeftoverTrackerBackend backend{""};
  json add_status =
      json::parse(backend.AddRecord(CreateDefaultQueryString()).dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add the leftover record.";
}

// Add a few records one after the other
TEST(LeftoverTrackerBackend, AddMultipleBasicRecords) {
  LeftoverTrackerBackend backend{""};

  json add_status =
      json::parse(backend.AddRecord(CreateDefaultQueryString()).dump());
  ASSERT_TRUE(add_status.at("success"))
      << "Failed to add the food wastage record record.";

  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithCost(20)).dump());
  ASSERT_TRUE(add_status.at("success")) << "Failed to add the second record.";

  add_status = json::parse(
      backend.AddRecord(CreateDefaultQueryStringWithFoodName("Tacos")).dump());
  ASSERT_TRUE(add_status.at("success")) << "Failed to add the third record.";
}

// Add duplicate records
TEST(LeftoverTrackerBackend, AddDuplicateRecord) {
  LeftoverTrackerBackend backend{""};

  json add_status =
      json::parse(backend.AddRecord(CreateDefaultQueryString()).dump());
  ASSERT_TRUE(add_status.at("success"));

  add_status =
      json::parse(backend.AddRecord(CreateDefaultQueryString()).dump());
  ASSERT_FALSE(add_status.at("success"))
      << "Don't add duplicate records. Check for the equality of the records "
         "and skip if a duplicate is already present in the vector.";
}

// Add large quantities of records to ensure efficiency
TEST(LeftoverTrackerBackend, AddManyRecords) {
  LeftoverTrackerBackend backend{""};

  for (int i = 0; i < BIG_N; i++) {
    json add_status = json::parse(
        backend.AddRecord(CreateDefaultQueryStringWithCost(i + 1)).dump());
    ASSERT_TRUE(add_status.at("success")) << "Failed to add record# " << i + 1;
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}
