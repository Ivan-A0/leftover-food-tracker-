from gggg import *

json_files = [
    "add_results.json",
    "records_results.json",
    "report_results.json",
    "write_results.json",
    "load_results.json",
    "test_results.json",
    "style_results.json"
]

add_record = Assignment(10, 4, gradescope_json_filename=json_files[0])
get_records = Assignment(10, 0, gradescope_json_filename=json_files[1])
get_report = Assignment(10, 0, gradescope_json_filename=json_files[2])
write_to_file = Assignment(10, 0, gradescope_json_filename=json_files[3])
load_from_file = Assignment(10, 0, gradescope_json_filename=json_files[4])
test = Assignment(10, 0, gradescope_json_filename=json_files[5])
code_style = Assignment(10, 0, gradescope_json_filename=json_files[6])

s_add_record = State(add_record)
s_add_record.reject_if_file_changed('main.cc',
                           '413336a411aa06c4619f75c660afd76b14e5761b8b4ca9b4923a1a1a01ffb6de')
s_add_record.reject_if_file_unchanged('leftover_record.h',
                           'ff6b98be4f0066c2793de3d3ad77d611f08236a251a159b04765cfb1062f168f')
s_add_record.reject_if_file_unchanged('leftover_report.h',
                           '48dba61f628a681d2a65510f794c7533aeb00d9b42825ebccc723d5e488c46f8')
s_add_record.reject_if_file_unchanged('leftover_tracker.h',
                           '24a2defedb36b2d40ca671beb515a191e0722d605e9b4dc2ed3b2d77731d4c83')
s_add_record.reject_if_file_unchanged('leftover_tracker_backend.h',
                           '8c8caa4003f6b2580b833cb72c779aa386f89b8d4cf2d875846661deb0332727')    
s_add_record.reject_if_file_unchanged('leftover_tracker_backend.cc',
                           '9eafab594651fbbe153d4d538fd87ea3222ab7789e2701fe0d7011413ef32ec4')                           

s_add_record.reject_if_file_changed('tools/settings/unittest_add_record.cc',
                         '7677cacae697ef7c9fd5770d43500a18db32f5fb5579f7f41aa6b005a8a59334')
s_add_record.reject_if_file_changed('tools/settings/unittest_delete_record.cc',
                         'fc37fc38530f454cf4f70e2919b5cf38763b54bc9d711c552cf253d54a32b446')
s_add_record.reject_if_file_changed('tools/settings/unittest_get_records.cc',
                         'c41d28c429642a414c2f6b076afc9373ef4f55f2565bbdafa2c31adb57eb57fb')
s_add_record.reject_if_file_changed('tools/settings/unittest_get_report.cc',
                         '5876716f51f04eb1eb5f11ef2f1f4aab1449f56763d9867eb1e74d56ef1e19d9')
s_add_record.reject_if_file_changed('tools/settings/unittest_load_from_file.cc',
                         'dcb85f42b76052c54e1b33c81ed93596440fe1ae493e7bf22e6a9e8ec54e547e')
s_add_record.reject_if_file_changed('tools/settings/unittest_write_to_file.cc',
                         '9d1cfbdd95759e7894be0d0b3a75ef75d55a9507d8b5850d4c15ecd74c023891')
s_add_record.reject_if_file_changed('tools/settings/unittest.cc',
                         '17363aead3085bb2d22d31a4449e8428133540744760f4c9a073763496c3ba67')                                                                                                                             
                         
s_add_record.gtest_run("tools/output/unittest_add_record")
s_add_record.gtest_suite_test("LeftoverTrackerBackend", 10)
s_add_record.summarize()

s_get_record = State(get_records)
s_get_record.gtest_run("tools/output/unittest_get_records")
s_get_record.gtest_suite_test("LeftoverTrackerBackend", 10)
s_get_record.summarize()

s_get_report = State(get_report)
s_get_report.gtest_run("tools/output/unittest_get_report")
s_get_report.gtest_suite_test("LeftoverTrackerBackend", 10)
s_get_report.summarize()

s_write_to_file = State(write_to_file)
s_write_to_file.gtest_run("tools/output/unittest_write_to_file")
s_write_to_file.gtest_suite_test("LeftoverTrackerBackend", 10)
s_write_to_file.summarize()

s_load_from_file = State(load_from_file)
s_load_from_file.gtest_run("tools/output/unittest_load_from_file")
s_load_from_file.gtest_suite_test("LeftoverTrackerBackend", 10)
s_load_from_file.summarize()

s_test = State(test)
s_test.gtest_run("tools/output/unittest")
s_test.gtest_suite_test("LeftoverTrackerBackend", 10)
s_test.summarize()

s_code_style = State(code_style)
s_code_style.clang_tidy_run("main.cc", "main", ["leftover_record.cc", "leftover_report.cc", "leftover_tracker_backend.cc", "leftover_tracker.cc"])
s_code_style.clang_tidy_check(5)
s_code_style.clang_format_run(["main.cc", "leftover_record.cc", "leftover_report.cc", "leftover_tracker_backend.cc", "leftover_tracker.cc", "leftover_record.h", "leftover_report.h", "leftover_tracker_backend.h", "leftover_tracker.h"])
s_code_style.clang_format_check(5)
s_code_style.summarize()

result = {'visibility': 'visible',
     'stdout_visibility': 'visible'}

for json_file in json_files:
    score = 0
    try:
        with open(json_file, 'r') as file:
            data = json.load(file)
            
            if 'tests' in result:
                if 'tests' in data:
                    result['tests'] += data['tests']
                    for test in data['tests']:
                        score += test['score']
                else:
                    score += data['score']
            else:
                if 'tests' in data:
                    result['tests'] = data['tests']
                    for test in data['tests']:
                        score += test['score']
                else:
                    score += data['score']
    except FileNotFoundError:
        print("\nError evaluating code. Please make sure your code compiles, your unit tests and other checks pass and then resubmit.\n")
    if 'score' in result:
        result['score'] += score
    else:
        result['score'] = score

with open("results.json", 'wt') as f:
    json.dump(result, f)
