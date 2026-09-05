add_executable(itpld_tests test.c)
target_link_libraries(itpld_tests PRIVATE itpld_core project_options)

add_test(NAME itpld_tests COMMAND itpld_tests)
