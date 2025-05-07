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

using ::testing::HasSubstr;
using ::testing::Not;
using json = nlohmann::json;

TEST(LeftoverTrackerBackend, LoadFromFile) {
  std::string kFilename = "tools/settings/data/leftover_records.json";
  LeftoverTrackerBackend backend{kFilename};
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
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}
