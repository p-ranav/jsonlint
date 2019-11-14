#include <jsonlint/parser.hpp>
#include <jsonlint/string.hpp>
#include <termcolor/termcolor.hpp>

namespace jsonlint {

namespace details {

unsigned int GetNumberOfDigits(unsigned int number) {
  unsigned int digits = 0;
  while (number) {
    number /= 10;
    digits += 1;
  }
  return digits;
}

void ReportError(Parser &context, Token start, Token end, const std::string &brief_descrition,
                 const std::string &detailed_description) {
  context.errors.push_back(std::make_tuple(start, end, brief_descrition, detailed_description));

  std::string file = start.filename;
  unsigned int line = start.line;
  unsigned int cursor = start.cursor_start + 1;

  std::string what = file + ":" + std::to_string(line) + ":" + std::to_string(cursor) + ": " +
                     brief_descrition + ". " + detailed_description;

  if (context.silent_mode) {
    throw std::runtime_error(what);
    return;
  }

  std::vector<unsigned int> line_numbers = {};
  if (line == 1) {
    line_numbers = std::vector<unsigned int>{line, (line + 1)};
  } else {
    line_numbers = std::vector<unsigned int>{(line - 1), line, (line + 1)};
  }
  unsigned int max_line_number = *(std::max_element(line_numbers.begin(), line_numbers.end()));
  std::string blanks(GetNumberOfDigits(max_line_number), ' ');

  std::string message_leading_blanks(cursor - 1, ' ');
  std::string message_carets = " ";

  if (end.cursor_start - start.cursor_end == 1) {
    message_carets = " " + std::string(start.cursor_end - start.cursor_start, '^') + " ";
  } else if (end.cursor_start - start.cursor_end > 1) {
    message_carets = " " + std::string(end.cursor_start - cursor - 1, '^') + " ";
  } else {
    message_carets = " " + std::string(start.cursor_end - start.cursor_start, '^') + " ";
  }

  if (message_carets == " " || message_carets == "  ") {
    message_carets = " ^ ";
  }

  std::vector<std::string> lines = string::Split(context.source, "\n");
  std::string error_line = lines[line - 1];

  std::cout << termcolor::red << termcolor::bold << "error: " << brief_descrition
            << termcolor::reset << std::endl;
  std::cout << termcolor::bold << blanks << "--> " << file << ":" << line << ":" << cursor
            << std::endl;

  if ((line - 1) > 0) {
    std::string line_leading_blanks = "";
    line_leading_blanks.insert(line_leading_blanks.begin(),
                               (GetNumberOfDigits(max_line_number) - GetNumberOfDigits(line - 1)),
                               ' ');
    std::cout << termcolor::bold << blanks << " |  " << std::endl;
    if ((line - 2) < lines.size())
      std::cout << termcolor::bold << line_leading_blanks << (line - 1) << " |  " << lines[line - 2]
                << std::endl;
    else
      std::cout << termcolor::bold << line_leading_blanks << (line - 1) << " |  " << std::endl;
  }

  std::string line_leading_blanks = "";
  line_leading_blanks.insert(line_leading_blanks.begin(),
                             (GetNumberOfDigits(max_line_number) - GetNumberOfDigits(line)), ' ');

  std::cout << termcolor::bold << blanks << " |  " << std::endl;
  std::cout << termcolor::bold << line_leading_blanks << line << " |  " << error_line << std::endl;
  std::cout << termcolor::bold << blanks << " | " << message_leading_blanks << termcolor::red
            << message_carets << detailed_description << termcolor::reset << std::endl;

  line_leading_blanks = "";
  line_leading_blanks.insert(line_leading_blanks.begin(),
                             (GetNumberOfDigits(max_line_number) - GetNumberOfDigits(line + 1)),
                             ' ');

  if ((line + 1) < lines.size())
    std::cout << termcolor::bold << line_leading_blanks << (line + 1) << " |  " << lines[line]
              << std::endl;
  else
    std::cout << termcolor::bold << line_leading_blanks << (line + 1) << " |  " << std::endl;

  std::cout << blanks << " |  " << std::endl;
  std::cout << std::endl;
  throw std::runtime_error(what);
}

bool ParseElement(Parser &context) {
  auto prefix = context.visitors[context.current.type];
  if (!prefix) {
    ReportError(context, context.current, context.peek, "Failed to parse element",
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
    ReportError(context, context.current, context.peek, "Failed to parse signed number",
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
      context.NextToken();
      ReportError(context, context.current, context.peek, "Failed to parse array",
                  "Expected ']', instead got ',' - You probably have an extra comma at the end of your list, e.g., [\"a\", \"b\",]");
    }
    context.NextToken(); // get past ','
    if (!ParseElement(context) || context.IsCurrentToken(TokenType::EOF_))
      return false;
  }
  if (!context.ExpectPeek(TokenType::RIGHT_BRACKET)) {
    context.NextToken();
    ReportError(context, context.current, context.peek, "Failed to parse array",
                "Expected ']', instead got '" + context.current.literal + "'");
    return false;
  }
  return true;
}

bool ParseObject(Parser &context) {
  // assume current is '{'
  if (context.IsPeekToken(TokenType::RIGHT_BRACE)) {
    context.NextToken();
    return true;
  }
  // there's at least one element
  context.NextToken(); // get past '{'
  while (!context.IsCurrentToken(TokenType::RIGHT_BRACE) && !context.IsCurrentToken(TokenType::EOF_)) {
    if (!context.IsCurrentToken(TokenType::STRING)) {
      ReportError(context, context.current, context.peek, "Failed to parse object",
                  "Expected STRING, instead got '" + context.current.literal + "'");
      return false;
    }
    // current is string key
    if (!context.ExpectPeek(TokenType::COLON)) {
      context.NextToken();
      ReportError(context, context.current, context.peek, "Failed to parse object",
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
      ReportError(context, context.current, context.peek, "Failed to parse object",
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
  // TODO: check if tokens is empty
  // Initialize current and peek token
  NextToken();
  NextToken();
  if (!IsPeekToken(TokenType::EOF_)) {
    if (!details::ParseElement(*this))
      return false;
  }
  if (!ExpectPeek(TokenType::EOF_)) {
    NextToken();
    details::ReportError(*this, current, peek, "Failed to parse JSON",
                         "Expected EOF, instead got '" + current.literal + "'");
    return false;
  }
  if (!silent_mode) {
    std::cout << current.filename << ": " << termcolor::green << termcolor::bold << "Valid JSON"
              << termcolor::reset << std::endl;
  }
  return true;
}

} // namespace jsonlint
