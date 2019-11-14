#include <jsonlint/parser.hpp>

namespace jsonlint {

namespace details {

bool ParseElement(Parser &context) {
  auto prefix = context.visitors[context.current.type];
  if (!prefix) {
    // TODO: report error
    return false;
  } else
    return prefix(context);
}

bool ParsePrimitive(Parser &context) {
  return true;
}

bool ParseArrayLiteral(Parser &context) {
  // assume current is '['
  if (context.IsPeekToken(TokenType::RIGHT_BRACKET))
    return true;
  // there's at least one element
  context.NextToken(); // get past '['
  if (!ParseElement(context)) return false;
  while (context.IsPeekToken(TokenType::COMMA)) {
    context.NextToken(); // current is ','
    context.NextToken(); // get past ','
    if (!ParseElement(context)) return false;
  }
  return true;
}

bool ParseObject(Parser &context) {
  // assume current is '{'
  if (context.IsPeekToken(TokenType::RIGHT_BRACE))
    return true;
  // there's at least one element
  context.NextToken(); // get past '{'
  while (!context.IsCurrentToken(TokenType::RIGHT_BRACE)) {
    if (!context.IsCurrentToken(TokenType::STRING)) {
      // TODO: report error
      return false;
    }
    // current is string key
    if (!context.ExpectPeek(TokenType::COLON)) {
      // TODO: report error
      return false;
    }
    // current is colon
    context.NextToken(); // get past ':'
    if (!ParseElement(context)) return false;
    if (!context.IsPeekToken(TokenType::RIGHT_BRACE) && !context.ExpectPeek(TokenType::COMMA)) {
      context.NextToken();
      // TODO: report error
      // ReportError(context, context.current, context.peek, "Failed to parse object",
      //             "Expected either '}' or ',', instead got '" + context.current.literal + "'");
      return false;
    }
    context.NextToken();    
  }    
  return true;
}
  
}  
  
Parser::Parser(const std::vector<Token>& tokens, const std::string &source) :
  current_index(0), current(Token{}), peek(Token{}), tokens(tokens), source(source), errors({}) {  
  RegisterVisitor(TokenType::NUMBER, details::ParsePrimitive);
  RegisterVisitor(TokenType::STRING, details::ParsePrimitive);
  RegisterVisitor(TokenType::TRUE, details::ParsePrimitive);
  RegisterVisitor(TokenType::FALSE, details::ParsePrimitive);
  RegisterVisitor(TokenType::NULL_, details::ParsePrimitive);
  // TODO: Add MINUS and PLUS
  RegisterVisitor(TokenType::LEFT_BRACKET, details::ParseArrayLiteral);
  RegisterVisitor(TokenType::LEFT_BRACE, details::ParseObject);
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
			     std::function<bool(Parser &)> function) {
  if (visitors.find(type) != visitors.end())
    visitors[type] = function;
  else
    visitors.insert(std::make_pair(type, function));
}

bool Parser::ParseJson() {
  // Initialize current and peek token
  NextToken();
  NextToken();
  if (!IsPeekToken(TokenType::EOF_)) {
    if (!details::ParseElement(*this)) return false;
  }
  // TODO: expect EOF
  return true;
}
  
}
