#pragma once
#include <string>
#include <vector>

namespace jsonlint {

enum class TokenType {
  TK_STRING,
  TK_NUMBER,
  TK_TRUE,
  TK_FALSE,
  TK_NULL,
  TK_COMMA,
  TK_PLUS,
  TK_MINUS,
  TK_LEFT_BRACKET,
  TK_RIGHT_BRACKET,
  TK_LEFT_BRACE,
  TK_RIGHT_BRACE,
  TK_ILLEGAL,
  TK_EOF
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
};

std::vector<Token> Tokenize(Lexer &context);

} // namespace jsonlint
