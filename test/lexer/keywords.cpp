#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
using namespace jsonlint;

TEST_CASE("Keyword 'true'", "[lexer]") {
  std::string filename = "";
  std::string source = "true";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::TRUE);
  REQUIRE(tokens[0].literal == source);
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == source.size() + 1);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Keyword 'false'", "[lexer]") {
  std::string filename = "";
  std::string source = "false";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::FALSE);
  REQUIRE(tokens[0].literal == source);
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == source.size() + 1);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Keyword 'null'", "[lexer]") {
  std::string filename = "";
  std::string source = "null";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].filename == "");
  REQUIRE(tokens[0].line == 1);
  REQUIRE(tokens[0].cursor_start == 1);
  REQUIRE(tokens[0].type == TokenType::NULL_);
  REQUIRE(tokens[0].literal == source);
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == source.size() + 1);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

#include <iostream>

TEST_CASE("Failed to parse unexpected keyword 'foo'", "[lexer]") {
  std::string filename = "";
  std::string source = "foo";
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
  REQUIRE(std::get<2>(errors[0]) == std::string{"Failed to parse keyword"});
  REQUIRE(std::get<3>(errors[0]) ==
          std::string{"Expected 'true', 'false', or 'null', instead got 'foo'"});
}
