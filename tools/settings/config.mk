## Unittest name
UTNAME_GET_RECORDS		:= unittest_get_records.cc
UTNAME_ADD_RECORD		:= unittest_add_record.cc
UTNAME_DELETE_RECORD		:= unittest_delete_record.cc
UTNAME_GET_REPORT		:= unittest_get_report.cc
UTNAME_LOAD_FROM_FILE		:= unittest_load_from_file.cc
UTNAME_WRITE_TO_FILE		:= unittest_write_to_file.cc
UTNAME		:= unittest.cc
UT_UTILS		:= test_utils.cc

# Flags added to compilation step
COMPILE_FLAGS		:=
# Flags added to unittest compilation step
UT_COMPILE_FLAGS	:=
# Flags added for mac compilation, if different from COMPILE_FLAGS
MAC_COMPILE_FLAGS	:=
# Flags added for mac unittest compilation step, if different from UT_COMPILE_FLAGS
MAC_UT_COMPILE_FLAGS :=
# Space-separated list of implementation files that should not be style/format
# checked, i.e. library definitions from cpputils.
OTHER_IMPLEMS	:=
# Space-separated list of header files (e.g., algebra.hpp)
HEADERS      		:= leftover_tracker_backend.h leftover_tracker.h leftover_report.h leftover_record.h
# Space-separated list of implementation files (e.g., algebra.cpp)
IMPLEMS       		:= leftover_tracker_backend.cc leftover_tracker.cc leftover_report.cc leftover_record.cc
# File containing main (e.g., main.cpp)
DRIVER        		:= main.cc
# Expected name of executable file
EXEC_FILE      		:= main
# Flags to pass to clang-format, for example, --style=Google. Use quotes around
# multiple flags. Optional.
CLANG_FORMAT_FLAGS	:= 
