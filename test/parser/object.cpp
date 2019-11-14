#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
#include <jsonlint/parser.hpp>
using namespace jsonlint;

TEST_CASE("The parser can parse the object {\"a\": 2}", "[parser]") {
  std::string filename = "";
  std::string source = "{\"a\": 2}";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the object {\"a\": \"Hello\"}", "[parser]") {
  std::string filename = "";
  std::string source = "{\"a\": \"Hello\"}";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can detect duplicate keys in {\"a\": 1, \"b\": 2, \"a\": 3, \"c\": 4}",
          "[parser]") {
  std::string filename = "";
  std::string source = "{\"a\": 1, \"b\": 2, \"a\": 3, \"c\": 4}";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  bool exception_thrown = false;
  try {
    auto result = parser.ParseJson();
  } catch (std::runtime_error &e) {
    exception_thrown = true;
  }
  auto errors = parser.errors;
  REQUIRE(errors.size() == 1);
  REQUIRE(std::get<2>(errors[0]) == std::string{"Failed to parse object"});
  REQUIRE(std::get<3>(errors[0]) == std::string{"Duplicate key 'a'"});
}
