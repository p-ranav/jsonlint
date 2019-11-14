#include <catch2/catch.hpp>
#include <clocale>
#include <jsonlint/lexer.hpp>
#include <jsonlint/parser.hpp>
using namespace jsonlint;

TEST_CASE("The parser can parse the array [1, 2, 3]", "[parser]") {
  std::string filename = "";
  std::string source = "[1, 2, 3]";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can detect the trailing comma in [1, 2, 3,]", "[parser]") {
  std::string filename = "";
  std::string source = "[1, 2, 3,]";
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
  REQUIRE(std::get<2>(errors[0]) == std::string{"Failed to parse array"});
  REQUIRE(std::get<3>(errors[0]) == std::string{"Expected ']', instead got ',' - You probably have an extra comma at the end of your list, e.g., [\"a\", \"b\",]"});
}

TEST_CASE("The parser can parse the array [1, 3.14, true, \"Hello\"]", "[parser]") {
  std::string filename = "";
  std::string source = "[1, 3.14, true, \"Hello\"]";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the array [1, 3.14, true, \"Hello\", [4, 5, 6]]", "[parser]") {
  std::string filename = "";
  std::string source = "[1, 3.14, true, \"Hello\", [4, 5, 6]]";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}

TEST_CASE("The parser can parse the array [{\"a\": 1, \"b\": [2, 3]}, {\"c\": true, \"d\": null}]",
          "[parser]") {
  std::string filename = "";
  std::string source = "[{\"a\": 1, \"b\": [2, 3]}, {\"c\": true, \"d\": null}]";
  Lexer lexer{"", 0, "", 1, 1};
  lexer.filename = filename;
  lexer.source = source;
  auto tokens = Tokenize(lexer);
  Parser parser(tokens, source);
  parser.silent_mode = true;
  auto result = parser.ParseJson();
  REQUIRE(result);
}
