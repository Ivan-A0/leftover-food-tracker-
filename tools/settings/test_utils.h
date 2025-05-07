#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <nlohmann/json.hpp>
#include <string>

#include "../../leftover_tracker_backend.h"

const int BIG_N =
    10;  // TODO: Push this to a higher number once all tests are written

// Default record constants
const std::string kDefaultDate = "2023-10-21";
const std::string kDefaultMeal = "Dinner";
const std::string kDefaultFood = "Pasta";
const double kDefaultQty = 5;
const std::string kDefaultDisposal = "Donation";
const double kDefaultCost = 4.5;
const std::string kDefaultLeftoverReason = "Too much left overs";

// Leftover reasons
const std::string kTooMuchLeftOverT = "Too much left overs";
const std::string kTastesBadT = "Tastes bad";
const std::string kExpiredT = "Expired";

// Leftover reduction strategies constants
const std::string kBuyLessFoodT = "Buy less food";
const std::string kCookSmallerServingsT = "Cook small servings";
const std::string kRecycleLeftOversT = "Recycle left overs";
const std::string kDonateBeforeExpirationT = "Donate before expiration";

// Disposal Mechanisms
const std::string kSinkT = "Sink";
const std::string kTrashT = "Trash";
const std::string kFeedToPetT = "Feed to pet";
const std::string kCompostT = "Compost";
const std::string kDonationT = "Donation";
const std::string kOtherT = "Other";

const std::string kTestFilename =
    "tools/settings/data/leftover_records.json";

crow::query_string CreateDefaultQueryString();

crow::query_string CreateDefaultQueryStringWithDate(std::string date);

crow::query_string CreateDefaultQueryStringWithFoodName(std::string food_name);

crow::query_string CreateDefaultQueryStringWithCost(double cost);

crow::query_string CreateDefaultQueryStringWithLeftoverReason(
    std::string leftover_reason);

crow::query_string CreateDefaultQueryStringWithMeal(std::string meal);

crow::query_string CreateDefaultQueryStringWithDisposalMechanism(
    std::string leftover_reason);

crow::query_string QueryStringfromRecordDetails(
    std::string date, std::string meal, std::string food_name, double qty,
    std::string disposal_mechanism, double cost, std::string leftover_reason);

bool jsonContains(nlohmann::json json, std::string strategy_string);

bool jsonEmpty(nlohmann::json json);

int jsonListLength(nlohmann::json json);

bool loadTrackerWithDefaultTestData(LeftoverTrackerBackend& tracker);

bool loadTrackerWithDefaultTestData(LeftoverTrackerBackend& tracker,
                                    const std::string& json_filename);

#endif