#ifndef LEFTOVER_TRACKER_BACKEND_H_
#define LEFTOVER_TRACKER_BACKEND_H_

#include <filesystem>
#include <string>

#include "leftover_tracker.h"
#include "server_utils/crow_all.h"

// A class to represent the leftover tracker backend. This class receives
// and returns CROW framework compatible objects. So when you are implementing
// your own classes to represent concepts like leftover record and leftover
//  report etc, you'll have to convert those objects to/from CROW
// objects. Some helper functions have been provided in
// leftover_tracker_backend.cc to help you with that conversion.
class LeftoverTrackerBackend {
 public:
  // yLeftoverTrackerBackend();
  //  Reads the leftover records from the file at
  //  kLeftoverRecordsFilepath, and loads those in memory. If the file is
  //  empty, it is a no-op. Returns true if the file was read successfully, else
  //  returns false. NOTE: This function should be called right after the object
  //  instantiation.
  //
  //  Example:
  // LeftoverTrackerBackend backend{"path/to/some_file_name.JSON"};
  // backend.LoadRecordsFromJsonFile();
  LeftoverTrackerBackend(const std::string& leftover_records_filepath)
      : kLeftoverRecordsFilepath{leftover_records_filepath} {}
  bool LoadRecordsFromJSONFile();

  // Writes the leftover records from the memory to a JSON file at path
  // kLeftoverRecordsFilepath. Returns true if the file was written
  // successfully, else returns false.
  bool WriteRecordsToJSONFile() const;

  // Add the given leftovere record (in the query_string) to the memory.
  crow::json::wvalue AddRecord(const crow::query_string& query_string);

  // Delete the given leftover record (in the query_string) from the memory.
  crow::json::wvalue DeleteRecord(const crow::query_string& query_string);

  // Returns all the leftover records that are stored in the memory.
  crow::json::wvalue GetRecords() const;

  // Generate a report based on all the leftover records in the memory.
  crow::json::wvalue GetLeftoverReport() const;

 private:
  // A top level class object to leftovers.
  // TODO: Add a LeftoverTracker object as a member variable here. Make sure you
  // stick to Google's style guide when naming your variable.

  // File path of the file that stores all the leftover records in JSON
  // format.
  const std::string kLeftoverRecordsFilepath;
  LeftoverTracker tracker_;
  // LeftoverTrackerBackend constants
  const char* kStatusSuccess = "success";
  const char* kNumRecords = "num_records";
  const char* kRecordArray = "record_array";
};

// Constants used in File Reading/Writing
extern const char* kFileDataDate;
extern const char* kFileDataMeal;
extern const char* kFileDataFoodName;
extern const char* kFileDataQtyInOz;
extern const char* kFileDataLeftoverReason;
extern const char* kFileDataFoodDisposalMechanism;
extern const char* kFileDataCost;

// Constants used in sending/receiving data from the web
extern const char* kWebDataDate;
extern const char* kWebDataMeal;
extern const char* kWebDataFoodName;
extern const char* kWebDataQtyInOz;
extern const char* kWebDataLeftoverReason;
extern const char* kWebDataDisposalMechanism;
extern const char* kWebDataCost;

// Constants used in sending/receiving report data from the web.
extern const char* kWebReportDataCommonDisposalMechanism;
extern const char* kWebReportDataCommonLeftOver;
extern const char* kWebReportDataCommonLeftOverReason;
extern const char* kWebReportDataMostLeftOverProducingMeal;
extern const char* kWebReportDataSuggestedReductLeftover;
extern const char* kWebReportDataLeftOverCost;

#endif
