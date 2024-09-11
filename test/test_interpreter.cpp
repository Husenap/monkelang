#include <fstream>

#include <gtest/gtest.h>

#include "Parser.hpp"
#include "Scanner.hpp"
#include "gtest/gtest.h"
#include "monke/frontend/interpreter/Interpreter.hpp"
#include "util.hpp"

class TestInterpreter : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(TestInterpreter, TestFileProcessing) {
  auto in_filepath  = GetParam();
  auto out_filepath = in_filepath;
  out_filepath.replace_extension("out");
  auto expected_filepath = in_filepath;
  expected_filepath.replace_extension("expected");
  auto read_filepath = in_filepath;
  read_filepath.replace_extension("read");

  std::ifstream in_file(in_filepath);
  std::ofstream out_file(out_filepath);
  std::ifstream read_file(read_filepath);

  monke::LexContext   ctx;
  monke::SourceModule sm;
  monke::Scanner      scanner{in_file, out_file};
  monke::Parser       parser{scanner, ctx, sm};

  ASSERT_EQ(parser.parse(), 0);

  monke::Interpreter interpreter(out_file, read_file);
  sm.visit(interpreter);

  ASSERT_EQ(ReadFile(out_filepath), ReadFile(expected_filepath));
}

INSTANTIATE_TEST_SUITE_P(AstFiles, TestInterpreter, ::testing::ValuesIn(GetTestFiles("testfiles/interpreter", ".in")));
