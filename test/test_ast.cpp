#include <fstream>

#include <gtest/gtest.h>

#include "Parser.hpp"
#include "Scanner.hpp"
#include "gtest/gtest.h"
#include "monke/frontend/ast/AstDumpTree.hpp"
#include "util.hpp"

class TestAst : public ::testing::TestWithParam<std::filesystem::path> {};

TEST_P(TestAst, TestFileProcessing) {
  auto in_filepath  = GetParam();
  auto out_filepath = in_filepath;
  out_filepath.replace_extension("out");
  auto expected_filepath = in_filepath;
  expected_filepath.replace_extension("expected");

  std::ifstream in_file(in_filepath);
  std::ofstream out_file(out_filepath);

  monke::LexContext   ctx;
  monke::SourceModule sm;
  monke::Scanner      scanner{in_file, out_file};
  monke::Parser       parser{scanner, ctx, sm};

  ASSERT_EQ(parser.parse(), 0);

  monke::AstDumpTree dumpTree(out_file);
  sm.visit(dumpTree);

  ASSERT_EQ(ReadFile(out_filepath), ReadFile(expected_filepath));
}

INSTANTIATE_TEST_SUITE_P(AstFiles, TestAst, ::testing::ValuesIn(GetTestFiles("testfiles/ast", ".in")));
