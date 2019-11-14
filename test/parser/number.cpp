#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
#include <jsonlint/parser.hpp>
using namespace jsonlint;

TEST_CASE("The parser can parse the number '0'", "[parser]") {
  std::string filename = "";
  std::string source = "0";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the number '-1'", "[parser]") {
  std::string filename = "";
  std::string source = "-1";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the number '+2'", "[parser]") {
  std::string filename = "";
  std::string source = "+2";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the number '123E-4'", "[parser]") {
  std::string filename = "";
  std::string source = "123E-4";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  auto result = parser.ParseJson();
  REQUIRE(result);
}
