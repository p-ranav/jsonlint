#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
using namespace jsonlint;

TEST_CASE("String \"Hello World\"", "[lexer]") {
  std::string filename = "";
  std::string source = "\"Hello World\"";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::TK_STRING);
  REQUIRE(tokens[0].literal == "Hello World");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 14);
  REQUIRE(tokens[1].type == TokenType::TK_EOF);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("String \"Hello 世界\"", "[lexer]") {
  setlocale(LC_ALL, "");
  std::string filename = "";
  std::string source = "\"Hello 世界\"";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::TK_STRING);
  REQUIRE(tokens[0].literal == "Hello 世界");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 11);
  REQUIRE(tokens[1].type == TokenType::TK_EOF);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("String \"ο Δίας\"", "[lexer]") {
  setlocale(LC_ALL, "");
  std::string filename = "";
  std::string source = "\"ο Δίας\"";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::TK_STRING);
  REQUIRE(tokens[0].literal == "ο Δίας");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 9);
  REQUIRE(tokens[1].type == TokenType::TK_EOF);
  REQUIRE(tokens[1].literal == "");
}
