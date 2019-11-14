#pragma once
#include <string>
#include <vector>

namespace jsonlint {

enum class TokenType {
  STRING,
  NUMBER,
  TRUE,
  FALSE,
  NULL_,
  COLON,
  COMMA,
  PLUS,
  MINUS,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  LEFT_BRACE,
  RIGHT_BRACE,
  ILLEGAL,
  EOF_
};

struct Token {
  TokenType type;
  std::string literal;
  std::string filename;
  int line;
  int cursor_start;
  int cursor_end;
};

struct Lexer {
  std::string source;
  int index;
  std::string filename;
  int line;
  int cursor;
  std::vector<std::tuple<Token, Token, std::string, std::string>> errors;
  bool silent_mode;
};

std::vector<Token> Tokenize(Lexer &context);

} // namespace jsonlint
