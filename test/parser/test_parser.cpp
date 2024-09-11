#include <fstream>

#include <gtest/gtest.h>

#include "Parser.hpp"
#include "Scanner.hpp"
#include "gtest/gtest.h"
#include "util.hpp"

template <bool should_fail>
void test_result(std::filesystem::path filepath) {
  std::ifstream f(filepath);

  monke::LexContext   ctx;
  monke::SourceModule sm;
  monke::Scanner      scanner{f, std::cout};
  monke::Parser       parser{scanner, ctx, sm};

  if constexpr (should_fail) {
    EXPECT_THROW(parser.parse(), monke::SyntaxError);
  } else {
    EXPECT_EQ(parser.parse(), 0);
  }
}

class ParserValid : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(ParserValid, TestFileProcessing) {
  test_result<false>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(ValidFiles, ParserValid, ::testing::ValuesIn(GetTestFiles("testfiles/parser", ".valid")));

class ParserError : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(ParserError, TestFileProcessing) {
  test_result<true>(GetParam());
}

INSTANTIATE_TEST_SUITE_P(ErrorFiles, ParserError, ::testing::ValuesIn(GetTestFiles("testfiles/parser", ".error")));
