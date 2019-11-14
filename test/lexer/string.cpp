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
  REQUIRE(tokens[0].type == TokenType::STRING);
  REQUIRE(tokens[0].literal == "Hello World");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 14);
  REQUIRE(tokens[1].type == TokenType::EOF_);
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
  REQUIRE(tokens[0].type == TokenType::STRING);
  REQUIRE(tokens[0].literal == "Hello 世界");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 11);
  REQUIRE(tokens[1].type == TokenType::EOF_);
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
  REQUIRE(tokens[0].type == TokenType::STRING);
  REQUIRE(tokens[0].literal == "ο Δίας");
  REQUIRE(tokens[1].filename == "");
  REQUIRE(tokens[1].line == 1);
  REQUIRE(tokens[1].cursor_start == 9);
  REQUIRE(tokens[1].type == TokenType::EOF_);
  REQUIRE(tokens[1].literal == "");
}

TEST_CASE("Incorrect unicode escape sequence", "[parser]") {
  std::string filename = "";
  std::string source = R"("\uABCG")";
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
  REQUIRE(std::get<2>(errors[0]) == std::string{"Failed to parse unicode escape sequence"});
  REQUIRE(std::get<3>(errors[0]) == std::string{"Expected hex character, instead got 'G'"});
}
