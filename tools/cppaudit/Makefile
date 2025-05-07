include ../settings/config.mk

OS_NAME 		:= $(shell uname -s | tr A-Z a-z)
SHELL         		:= bash
OUTPUT_PATH		:= ../output
SETTINGS_PATH		:= ../settings
ROOT_PATH		:= $(shell printf "%q" "$(PWD)")
CPP_AUDIT_PATH		:= $(shell printf "%q" "$$(pwd)")
CPPAUDIT_FROM_ROOT 	:= $(shell realpath --relative-to=$(ROOT_PATH) $(CPP_AUDIT_PATH))
REL_ROOT_PATH		:= $(shell realpath --relative-to=$(CPP_AUDIT_PATH) $(ROOT_PATH))
OUTPUT_FROM_ROOT 	:= $(shell realpath --relative-to=$(ROOT_PATH) $(OUTPUT_PATH))
FILES         		:= $(DRIVER) $(IMPLEMS) $(HEADERS)
HAS_CLANGTDY  		:= $(shell command -v clang-tidy 2> /dev/null)
HAS_CLANGFMT  		:= $(shell command -v clang-format 2> /dev/null)
HAS_GTEST         	:= $(shell echo -e "int main() { }" >> test.cc ; clang++ test.cc -o test -lgtest 2> /dev/null; echo $$?; rm -rf test.cc test;)

PYTHON=python3
ifneq (, $(shell which python3.7))
	PYTHON=python3.7
endif
ifneq (, $(shell which python3.8))
	PYTHON=python3.8
endif

ifeq ($(OS_NAME), darwin)
	COMPILE_FLAGS	:= $(MAC_COMPILE_FLAGS)
	UT_COMPILE_FLAGS	:= $(MAC_UT_COMPILE_FLAGS)
	HAS_BREW	:= $(shell command -v brew 2> /dev/null)
endif

.PHONY: build test stylecheck formatcheck all clean noskiptest install_gtest test_add_record test_delete_record test_get_records test_get_report test_load_from_file test_write_to_file grade

$(OUTPUT_PATH):
	@mkdir -p $(OUTPUT_PATH)

$(OUTPUT_PATH)/unittest_add_record: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME_ADD_RECORD) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS)) $(SETTINGS_PATH)/$(UT_UTILS)
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME_ADD_RECORD) -o $(OUTPUT_PATH)/unittest_add_record -pthread -lgtest $(UT_COMPILE_FLAGS)

$(OUTPUT_PATH)/unittest_delete_record: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME_DELETE_RECORD) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS))
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME_DELETE_RECORD) -o $(OUTPUT_PATH)/unittest_delete_record -pthread -lgtest $(UT_COMPILE_FLAGS)

$(OUTPUT_PATH)/unittest_get_records: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME_GET_RECORDS) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS))
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME_GET_RECORDS) -o $(OUTPUT_PATH)/unittest_get_records -pthread -lgtest $(UT_COMPILE_FLAGS)

$(OUTPUT_PATH)/unittest_get_report: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME_GET_REPORT) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS))
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME_GET_REPORT) -o $(OUTPUT_PATH)/unittest_get_report -pthread -lgtest $(UT_COMPILE_FLAGS)

$(OUTPUT_PATH)/unittest_load_from_file: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME_LOAD_FROM_FILE) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS))
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME_LOAD_FROM_FILE) -o $(OUTPUT_PATH)/unittest_load_from_file -pthread -lgtest $(UT_COMPILE_FLAGS)

$(OUTPUT_PATH)/unittest_write_to_file: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME_WRITE_TO_FILE) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS))
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME_WRITE_TO_FILE) -o $(OUTPUT_PATH)/unittest_write_to_file -pthread -lgtest $(UT_COMPILE_FLAGS)

$(OUTPUT_PATH)/unittest: $(OUTPUT_PATH) $(SETTINGS_PATH)/$(UTNAME) $(addprefix $(REL_ROOT_PATH)/, $(DRIVER) $(IMPLEMS) $(HEADERS))
	@clang++ -std=c++17 -fsanitize=address $(addprefix $(REL_ROOT_PATH)/, $(IMPLEMS) $(OTHER_IMPLEMS)) $(SETTINGS_PATH)/$(UT_UTILS) $(SETTINGS_PATH)/$(UTNAME) -o $(OUTPUT_PATH)/unittest -pthread -lgtest $(UT_COMPILE_FLAGS)


install_gtest:
ifeq ($(HAS_GTEST),1)
	@echo -e "google test not installed\n"
ifeq ($(OS_NAME), darwin)
ifndef ($(HAS_BREW))
	@echo -e "Installing brew, please provide your password when asked.\n"
	@/bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
	@brew install coreutils
	# Need to redefine the path directories otherwise this will fail the first time.
endif
	@echo -e "Installing cmake. Please provide the password when asked\n"
	@brew install cmake
	@echo -e "\nDownloading and installing googletest\n"
	@cd /tmp/; git clone https://github.com/google/googletest; cd googletest; mkdir build; cd build; cmake .. -DCMAKE_CXX_STANDARD=17; make; make install
	@echo -e "Finished installing google test\n"
else
ifneq ($(shell lsb_release -sr), 20.04)
	@cd /tmp/; git clone https://github.com/google/googletest.git; cd googletest; cmake CMakeLists.txt; make; sudo cp -r googletest/include/. /usr/include; sudo cp -r googlemock/include/. /usr/include; sudo cp lib/*.a /usr/lib
else
	@echo -e "Installing cmake. Please provide the password when asked\n"
	@sudo apt-get install cmake # install cmake
	@echo -e "\nDownloading and installing googletest\n"
	@sudo apt-get install libgtest-dev libgmock-dev
	@echo -e "Finished installing google test\n"
endif
endif
endif

build:
	@echo "Building the Leftover Food Tracker Backend. This may take a minute..."
	@cd $(ROOT_PATH)/ && g++ -g -Wno-cpp -O0 -o main $(DRIVER) $(IMPLEMS) -lpthread -std=c++17
	@echo "Successfully compiled the Food Wastage Tracker Backend!"

test: install_gtest $(OUTPUT_PATH)/unittest
	@echo -e "\n========================\nRunning unit test\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest.xml"
	@echo -e "\n========================\nUnit test complete\n========================\n"

test_add_record: install_gtest $(OUTPUT_PATH)/unittest_add_record
	@echo -e "\n========================\nRunning add record unit tests\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest_add_record --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest_add_record.xml"
	@echo -e "\n========================\nUnit tests complete\n========================\n"

test_delete_record: install_gtest $(OUTPUT_PATH)/unittest_delete_record
	@echo -e "\n========================\nRunning delete record unit tests\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest_delete_record --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest_delete_record.xml"
	@echo -e "\n========================\nUnit tests complete\n========================\n"

test_get_records: install_gtest $(OUTPUT_PATH)/unittest_get_records
	@echo -e "\n========================\nRunning get records unit tests\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest_get_records --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest_get_records.xml"
	@echo -e "\n========================\nUnit tests complete\n========================\n"

test_get_report: install_gtest $(OUTPUT_PATH)/unittest_get_report
	@echo -e "\n========================\nRunning get report unit tests\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest_get_report --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest_get_report.xml"
	@echo -e "\n========================\nUnit tests complete\n========================\n"

test_load_from_file: install_gtest $(OUTPUT_PATH)/unittest_load_from_file
	@echo -e "\n========================\nRunning load from file unit tests\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest_load_from_file --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest_load_from_file.xml"
	@echo -e "\n========================\nUnit tests complete\n========================\n"

test_write_to_file: install_gtest $(OUTPUT_PATH)/unittest_write_to_file
	@echo -e "\n========================\nRunning write to file unit tests\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest_write_to_file --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest_write_to_file.xml"
	@echo -e "\n========================\nUnit tests complete\n========================\n"

noskiptest: install_gtest $(OUTPUT_PATH)/unittest
	@echo -e "\n========================\nRunning unit test\n========================\n"
	@cd $(REL_ROOT_PATH)/ && ./$(OUTPUT_FROM_ROOT)/unittest --noskip --gtest_output="xml:$(OUTPUT_FROM_ROOT)/unittest.xml"
	@echo -e "\n========================\nUnit test complete\n========================\n"

$(OUTPUT_PATH)/compile_commands.json : $(SETTINGS_PATH)/config.mk
	@cd $(ROOT_PATH)/ && bash $(CPP_AUDIT_PATH)/gen_ccjs.sh $(OUTPUT_FROM_ROOT) $(EXEC_FILE) $(DRIVER) $(IMPLEMS)

stylecheck: $(OUTPUT_PATH)/compile_commands.json
ifndef HAS_CLANGTDY
	@echo -e "clang-tidy not installed\n"
	@echo -e "Installing clang-tidy. Please provide password when asked\n"
ifeq ($(OS_NAME),darwin)
	@echo -e "WARNING: Installing llvm to provide stylecheck functionality will take about an hour."
	@brew install llvm
	@ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
else
	@sudo apt-get -y install clang-tidy
	@echo -e "Finished installing clang-tidy\n"
endif
endif
	@echo -e "========================\nRunning style checker\n========================\n"
	@rm -f $(OUTPUT_PATH)/style.yaml
	@cd $(REL_ROOT_PATH)/ && clang-tidy -p=$(OUTPUT_FROM_ROOT) -quiet -header-filter=^crow_all.h  -export-fixes=$(OUTPUT_FROM_ROOT)/style.yaml $(IMPLEMS) $(DRIVER)
	@if [ -f "$(OUTPUT_PATH)/style.yaml" ]; then \
    echo -e "========================\nStyle checker failed\n========================\n"; \
    exit 1; \
  else \
    echo -e "========================\nStyle checker complete\n========================\n"; \
  fi
	@make clean

formatcheck:
ifndef HAS_CLANGFMT
ifeq ($(OS_NAME),darwin)
	@echo -e "clang-format not installed.\n"
	@echo -e "Installing clang-format. Please provide the password when asked\n"
	@brew install clang-format
	@echo -e "Finished installing clang-format\n"
else
	@echo -e "clang-format not installed.\n"
	@echo -e "Installing clang-format. Please provide the password when asked\n"
	@sudo apt-get -y install clang-format
	@echo -e "Finished installing clang-format\n"
endif
endif
	@echo -e "========================\nRunning format checker\n========================"
	@cd $(REL_ROOT_PATH)/ && bash $(CPPAUDIT_FROM_ROOT)/diff_format.sh $(CLANG_FORMAT_FLAGS) $(FILES)
	@cd $(REL_ROOT_PATH)/ && clang-format $(FILES) -output-replacements-xml > $(OUTPUT_FROM_ROOT)/format.xml
	@echo -e "========================\nFormat checking complete\n========================\n"

all: test_add_record test_get_records test_get_report test_load_from_file test_write_to_file test stylecheck formatcheck

grade: $(OUTPUT_PATH)/unittest_add_record $(OUTPUT_PATH)/unittest_delete_record $(OUTPUT_PATH)/unittest_get_records $(OUTPUT_PATH)/unittest_get_report $(OUTPUT_PATH)/unittest_load_from_file $(OUTPUT_PATH)/unittest_write_to_file $(OUTPUT_PATH)/unittest
	@-cd $(REL_ROOT_PATH)/ && pwd && ${PYTHON} $(CPP_AUDIT_PATH)/grade.py

clean:
	@rm -f $(OUTPUT_PATH)/*
