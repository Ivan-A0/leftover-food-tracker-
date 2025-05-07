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

TEST(LeftoverTrackerBackend, WriteToFile) {
  const std::string kFilename = "tools/settings/data/temp.json";
  LeftoverTrackerBackend backend{kFilename};
  backend.AddRecord(CreateDefaultQueryString());
  ASSERT_TRUE(backend.WriteRecordsToJSONFile())
      << "Tracker failed to write records to JSON.";

  LeftoverTrackerBackend backend2{kFilename};
  ASSERT_TRUE(loadTrackerWithDefaultTestData(backend2, kFilename));
  json records = json::parse(backend2.GetRecords().dump());
  ASSERT_TRUE(records.at("num_records") == 1)
      << "Failed to rerieve the written record from the JSON file. Double "
         "check to make sure that you followed all the steps required for this "
         "feature.";
  std::remove(kFilename.c_str());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}
