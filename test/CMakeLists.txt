add_executable(monke_test "parser/test_parser.cpp" "parser/test_ast.cpp")

target_link_libraries(monke_test PUBLIC monke_parser GTest::gtest_main)
target_include_directories(monke_test PUBLIC ${CMAKE_SOURCE_DIR}/src)

gtest_discover_tests(monke_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
