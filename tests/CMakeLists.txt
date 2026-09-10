add_executable(itpld_test
  testconf.c
  common/checked_test.c
  common/diagnostics_test.c
  common/file_test.c
  cli/options_test.c)

target_include_directories(itpld_test
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_compile_definitions(itpld_test
  PRIVATE ITPLD_TESTS_FIXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures")

target_compile_options(itpld_test PRIVATE -UNDEBUG)

target_link_libraries(itpld_test PRIVATE itpld_core project_options)

# sync the test list with the test registry in testconf.c
set(ALL_TESTS
  in_range
  int_size_cast
  long_size_cast
  size_add
  size_mul
  filebuf_init
  filebuf_destroy
  file_read
  status_str
  error_report
  cliopts_init
  cliopts_destroy
  cliopts_parse)

foreach(test IN LISTS ALL_TESTS)
  add_test(NAME "itpld_${test}" COMMAND itpld_test "${test}")
endforeach()
