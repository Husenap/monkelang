file(GLOB TEST_FILES "testfiles/ast/*.in" "testfiles/ast/*.expected"
     "testfiles/parser/*.valid" "testfiles/parser/*.error")

add_custom_target(monke_test_testfiles ALL DEPENDS ${TEST_FILES})

add_executable(monke_test "parser/test_parser.cpp" "parser/test_ast.cpp")

add_dependencies(monke_test monke_test_testfiles)

target_link_libraries(monke_test PUBLIC monke_parser GTest::gtest_main)
target_include_directories(monke_test PUBLIC ${CMAKE_SOURCE_DIR}/src)

gtest_discover_tests(monke_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
