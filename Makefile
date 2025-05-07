TARGETS = build test stylecheck formatcheck all noskiptest grade clean test_add_record test_delete_record test_get_records test_get_report test_load_from_file test_write_to_file

.PHONY: $(TARGETS)

tools/output:
	@mkdir -p tools/output

$(TARGETS): tools/output
	@$(MAKE) -C tools/cppaudit $(MAKECMDGOALS)