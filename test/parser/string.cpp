#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
#include <jsonlint/parser.hpp>
using namespace jsonlint;

TEST_CASE("The parser can parse the number 'Hello World'", "[parser]") {
  std::string filename = "";
  std::string source = "\"Hello World\"";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the number '\uABCD'", "[parser]") {
  std::string filename = "";
  std::string source = "\"\uABCD\"";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}
