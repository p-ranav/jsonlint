#pragma once

namespace jsonlint {

enum class TokenType {
  TK_STRING,
  TK_NUMBER,
  TK_TRUE,
  TK_FALSE,
  TK_NULL,
  TK_LEFT_BRACKET,
  TK_RIGHT_BRACKET,
  TK_LEFT_BRACE,
  TK_RIGHT_BRACE,
  TK_EOF
};

struct Token {
  TokenType type;
  std::string literal;
  std::string filename;
  int line;
  int cursor_start;
  int cursor_end;

  Token()
      : type(TokenType::ILLEGAL), literal(""), filename(""), line(1), cursor_start(0),
        cursor_end(0) {}
  Token(TokenType type, const std::string &literal, const std::string &filename, int line,
        int cursor_start)
      : type(type), literal(literal), filename(filename), line(line), cursor_start(cursor_start),
        cursor_end(cursor_start) {}
}

} // namespace jsonlint
