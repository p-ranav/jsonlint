#pragma once
#include <jsonlint/lexer.hpp>
#include <map>
#include <string>
#include <vector>
#include <functional>

namespace jsonlint {

enum class Element {
  STRING,
  NUMBER,
  TRUE,
  FALSE,
  NULL_,
  ARRAY,
  OBJECT
};

struct Parser {
  std::vector<Token> tokens;
  std::string source;
  unsigned int current_index;
  Token current;
  Token peek;
  std::map<TokenType, std::function<bool(Parser &)>> visitors;  
  std::vector<std::tuple<Token, Token, std::string, std::string>> errors;
  bool silent_mode;
  explicit Parser(const std::vector<Token> &tokens, const std::string &source);
  void NextToken();
  bool IsCurrentToken(TokenType value);
  bool IsPeekToken(TokenType value);
  bool ExpectPeek(TokenType value);
  bool ParseJson();
  void RegisterVisitor(TokenType, std::function<bool(Parser &)>);  
};
  
}
