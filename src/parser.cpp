#include <jsonlint/errors.hpp>
#include <jsonlint/parser.hpp>
#include <jsonlint/string.hpp>
#include <set>
#include <termcolor/termcolor.hpp>

namespace jsonlint {

namespace details {

bool ParseElement(Parser &context) {
  auto prefix = context.visitors[context.current.type];
  if (!prefix) {
    ReportParserError(context, context.current, context.peek, "Failed to parse element",
                      "Unexpected token '" + context.current.literal + "'");
    return false;
  } else {
    return prefix(context);
  }
}

bool ParsePrimitive(Parser &context) { return true; }

bool ParseSignedNumber(Parser &context) {
  // assume current is '-' or '+'
  if (!context.ExpectPeek(TokenType::NUMBER)) {
    context.NextToken();
    ReportParserError(context, context.current, context.peek, "Failed to parse signed number",
                      "Expected NUMBER, instead got '" + context.current.literal + "'");
    return false;
  }
  // current is number
  return true;
}

bool ParseArrayLiteral(Parser &context) {
  // assume current is '['
  if (context.IsPeekToken(TokenType::RIGHT_BRACKET)) {
    context.NextToken();
    return true;
  }
  // there's at least one element
  context.NextToken(); // get past '['
  if (!ParseElement(context))
    return false;
  while (context.IsPeekToken(TokenType::COMMA)) {
    context.NextToken(); // current is ','
    if (context.IsPeekToken(TokenType::RIGHT_BRACKET)) {
      // comma was a trailing comma, e.g., [1, 2, 3, ]
      //                                           ^^^ we're here
      ReportParserError(context, context.current, context.current, "Failed to parse array",
                        "Expected ']', instead got ',' - You probably have an extra comma at the "
                        "end of your list, e.g., [\"a\", \"b\",]");
    }
    context.NextToken(); // get past ','
    if (!ParseElement(context) || context.IsCurrentToken(TokenType::EOF_))
      return false;
  }
  if (!context.ExpectPeek(TokenType::RIGHT_BRACKET)) {
    context.NextToken();
    ReportParserError(context, context.current, context.peek, "Failed to parse array",
                      "Expected ']', instead got '" + context.current.literal + "'");
    return false;
  }
  return true;
}

bool ParseObject(Parser &context) {
  // assume current is '{'
  std::set<std::string> keys;
  if (context.IsPeekToken(TokenType::RIGHT_BRACE)) {
    context.NextToken();
    return true;
  }
  // there's at least one element
  context.NextToken(); // get past '{'
  while (!context.IsCurrentToken(TokenType::RIGHT_BRACE) &&
         !context.IsCurrentToken(TokenType::EOF_)) {
    if (!context.IsCurrentToken(TokenType::STRING)) {
      ReportParserError(context, context.current, context.peek, "Failed to parse object",
                        "Expected STRING, instead got '" + context.current.literal + "'");
      return false;
    }
    if (keys.find(context.current.literal) == keys.end())
      keys.insert(context.current.literal);
    else {
      ReportParserError(context, context.current, context.peek, "Failed to parse object",
                        "Duplicate key '" + context.current.literal + "'");
    }
    // current is string key
    if (!context.ExpectPeek(TokenType::COLON)) {
      context.NextToken();
      ReportParserError(context, context.current, context.peek, "Failed to parse object",
                        "Expected ':', instead got '" + context.current.literal + "'");
      return false;
    }
    // current is colon
    context.NextToken(); // get past ':'
    auto value = ParseElement(context);
    if (!value)
      return false;
    if (!context.IsPeekToken(TokenType::RIGHT_BRACE) && !context.ExpectPeek(TokenType::COMMA)) {
      context.NextToken();
      ReportParserError(context, context.current, context.peek, "Failed to parse object",
                        "Expected '}', instead got '" + context.current.literal + "'");
      return false;
    }
    context.NextToken();
  }
  return true;
}

} // namespace details

Parser::Parser(const std::vector<Token> &tokens, const std::string &source)
    : current_index(0), current(Token{}), peek(Token{}), tokens(tokens), source(source), errors({}),
      silent_mode(false) {
  RegisterVisitor(TokenType::NUMBER, details::ParsePrimitive);
  RegisterVisitor(TokenType::STRING, details::ParsePrimitive);
  RegisterVisitor(TokenType::TRUE, details::ParsePrimitive);
  RegisterVisitor(TokenType::FALSE, details::ParsePrimitive);
  RegisterVisitor(TokenType::NULL_, details::ParsePrimitive);
  RegisterVisitor(TokenType::PLUS, details::ParseSignedNumber);
  RegisterVisitor(TokenType::MINUS, details::ParseSignedNumber);
  RegisterVisitor(TokenType::LEFT_BRACKET, details::ParseArrayLiteral);
  RegisterVisitor(TokenType::LEFT_BRACE, details::ParseObject);
}

void Parser::PreviousToken() {
  peek = current;
  current_index -= 1;
  current = tokens[tokens.size() - 1]; // last token
  if (current_index - 2 < tokens.size())
    current = tokens[current_index - 2];
  else
    current = tokens[0];
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

void Parser::RegisterVisitor(TokenType type, std::function<bool(Parser &)> function) {
  if (visitors.find(type) != visitors.end())
    visitors[type] = function;
  else
    visitors.insert(std::make_pair(type, function));
}

bool Parser::ParseJson() {
  if (!tokens.empty()) {
    // Initialize current and peek token
    NextToken();
    NextToken();
    if (!IsPeekToken(TokenType::EOF_)) {
      if (!details::ParseElement(*this))
        return false;
    }
    if (!ExpectPeek(TokenType::EOF_)) {
      NextToken();
      details::ReportParserError(*this, current, peek, "Failed to parse JSON",
                                 "Expected EOF, instead got '" + current.literal + "'");
      return false;
    }
  }
  if (!silent_mode) {
    std::cout << termcolor::green << termcolor::bold << "Valid JSON" << termcolor::reset
              << std::endl;
  }
  return true;
}

} // namespace jsonlint
