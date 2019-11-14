#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
using namespace jsonlint;

TEST_CASE("Comma ','", "[lexer]") {
  std::string filename = "";
  std::string source = ",";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::COMMA);
  REQUIRE(tokens[0].literal == ",");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Left Brace '{'", "[lexer]") {
  std::string filename = "";
  std::string source = "{";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::LEFT_BRACE);
  REQUIRE(tokens[0].literal == "{");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Left Bracket '['", "[lexer]") {
  std::string filename = "";
  std::string source = "[";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::LEFT_BRACKET);
  REQUIRE(tokens[0].literal == "[");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Right Brace '}'", "[lexer]") {
  std::string filename = "";
  std::string source = "}";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::RIGHT_BRACE);
  REQUIRE(tokens[0].literal == "}");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Right Bracket ']'", "[lexer]") {
  std::string filename = "";
  std::string source = "]";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::RIGHT_BRACKET);
  REQUIRE(tokens[0].literal == "]");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}
