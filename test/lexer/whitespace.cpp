#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
using namespace jsonlint;

TEST_CASE("Whitespace ' '", "[lexer]") {
  std::string filename = "";
  std::string source = " ";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 2);
  REQUIRE(tokens[0].type == TokenType::EOF_);
  REQUIRE(tokens[0].literal == "");
}

TEST_CASE("Whitespace '    '", "[lexer]") {
  std::string filename = "";
  std::string source = "    ";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 5);
  REQUIRE(tokens[0].type == TokenType::EOF_);
  REQUIRE(tokens[0].literal == "");
}
