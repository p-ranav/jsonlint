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

TEST_CASE("Colon ':'", "[lexer]") {
  std::string filename = "";
  std::string source = ":";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::COLON);
  REQUIRE(tokens[0].literal == ":");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Plus '+'", "[lexer]") {
  std::string filename = "";
  std::string source = "+";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::PLUS);
  REQUIRE(tokens[0].literal == "+");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 2);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Minus '-'", "[lexer]") {
  std::string filename = "";
  std::string source = "-";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::MINUS);
  REQUIRE(tokens[0].literal == "-");
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

TEST_CASE("Unexpected punctuation", "[lexer]") {
  std::string filename = "";
  std::string source = R"({"a": 1))";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  lexer.silent_mode = true;
  bool exception_thrown = false;
  try {
    auto tokens = Tokenize(lexer);
  } catch (std::runtime_error &e) {
    exception_thrown = true;
  }
  auto errors = lexer.errors;
  REQUIRE(errors.size() == 1);
  REQUIRE(std::get<2>(errors[0]) == std::string{"Failed to parse punctuation"});
  REQUIRE(std::get<3>(errors[0]) == std::string{"Unexpected token ')'"});
}
