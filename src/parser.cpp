#include <jsonlint/parser.hpp>

namespace jsonlint {

Parser::Parser(const std::vector<Token>& tokens, const std::string &source) :
  current_index(0), current(Token{}), peek(Token{}), tokens(tokens), source(source), errors({}) {  
  
}

void Parser::NextToken() {
  current = peek;
  current_index += 1;
  peek = tokens[tokens.size() - 1];
  if (current_index - 1 < tokens.size())
    peek = tokens[current_index - 1];
}

bool Parser::IsCurrentToken(TokenType value) { return (current.type == value); }

bool Parser::IsPeekToken(TokenType value) { return (peek.type == value); }

bool Parser::ExpectPeek(TokenType value) {
  if (IsPeekToken(value)) {
    NextToken();
    return true;
  } else {
    return false;
  }
}

void Parser::RegisterVisitor(TokenType type,
			     std::function<void(Parser &)> function) {
  if (visitors.find(type) != visitors.end())
    visitors[type] = function;
  else
    visitors.insert(std::make_pair(type, function));
}

bool Parser::ParseJson() {
  // Initialize current and peek token
  NextToken();
  NextToken();

  while (!IsPeekToken(TokenType::EOF_)) {
    auto prefix = visitors[current.type];
    if (!prefix) {
      // TODO: report error
      return false;
    }
    prefix(*this);
  }
  return true;
}
  
}
