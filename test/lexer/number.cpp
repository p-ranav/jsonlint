#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
using namespace jsonlint;

TEST_CASE("Integer '0'", "[lexer]") {
  std::string filename = "";
  std::string source = "0";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "0");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Integer '5'", "[lexer]") {
  std::string filename = "";
  std::string source = "5";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "5");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Integer '42'", "[lexer]") {
  std::string filename = "";
  std::string source = "42";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "42");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 3);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Integer '-96'", "[lexer]") {
  std::string filename = "";
  std::string source = "-96";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 3);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::MINUS);
  REQUIRE(tokens[0].literal == "-");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::NUMBER);
  REQUIRE(tokens[1].literal == "96");
  REQUIRE(tokens[2].filename == "");
  REQUIRE(tokens[2].line == 1);
  REQUIRE(tokens[2].cursor_start == 4);
  REQUIRE(tokens[2].type == TokenType::EOF_);
  REQUIRE(tokens[2].literal == "");
}

TEST_CASE("Double '0.0'", "[lexer]") {
  std::string filename = "";
  std::string source = "0.0";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "0.0");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 4);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Double '3.14'", "[lexer]") {
  std::string filename = "";
  std::string source = "3.14";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "3.14");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 5);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Double '2.71828'", "[lexer]") {
  std::string filename = "";
  std::string source = "2.71828";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "2.71828");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 8);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Double '1.61803398875'", "[lexer]") {
  std::string filename = "";
  std::string source = "1.61803398875";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NUMBER);
  REQUIRE(tokens[0].literal == "1.61803398875");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 14);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}
