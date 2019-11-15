#include <iostream>
#include <jsonlint/errors.hpp>
#include <jsonlint/lexer.hpp>
#include <jsonlint/utf8.hpp>

namespace jsonlint {

namespace details {

std::string ReadCharacter(Lexer &context, bool update_index = true) {
  std::string result = "";
  int length = 0;
  // get length of next multi-byte character
  if (context.index < context.source.size())
    length = GetUTF8SequenceLength(&(context.source[context.index]));
  // append bytes of next multi-byte character to result
  for (int i = 0; i < length; i++, context.index++)
    result += context.source[context.index];
  // update index if necessary
  if (!update_index)
    context.index -= length;
  else
    context.cursor += 1;
  return result;
}

std::string PeekCharacter(Lexer &context) { return ReadCharacter(context, false); }

bool IsHexCharacter(const std::string &character) {
  if (character.size() == 0)
    return false;
  if (character.size() == 1) {
    auto c = character[0];
    return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' ||
            c == '7' || c == '8' || c == '9' || c == 'a' || c == 'b' || c == 'c' || c == 'd' ||
            c == 'e' || c == 'f' || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' ||
            c == 'F');
  }
  return false;
}

Token ReadString(Lexer &context) {
  Token token{TokenType::STRING, "", context.filename, context.line, context.cursor};
  // consume first double quote
  auto peek = ReadCharacter(context);
  while (true) {
    // peek at next character
    peek = PeekCharacter(context);
    if (peek[0] == 0x0A || peek[0] == EOF || peek == "") {
      token.cursor_start = context.cursor;
      token.cursor_end = context.cursor;
      ReportLexerError(context, token, token, "Failed to parse string", "Unterminated string");
    }
    // check if peek is the start of an escape sequence
    if (peek[0] == '\\') {
      // consume escape character
      peek = ReadCharacter(context);
      // check what comes next
      peek = PeekCharacter(context);
      if (peek[0] == '"') {
        peek = ReadCharacter(context);
        // escaped double quote character
        token.literal += "\\" + peek;
        continue;
      } else if (peek[0] == '\\') {
        peek = ReadCharacter(context);
        // escaped backslash character
        token.literal += "\\" + peek;
        continue;
      } else if (peek[0] == 'b' || // backspace
                 peek[0] == 'f' || // form feed
                 peek[0] == 'n' || // newline
                 peek[0] == 'r' || // carriage return
                 peek[0] == 't') { // horizontal tab
        peek = ReadCharacter(context);
        token.literal += "\\" + peek;
        continue;
      } else if (peek[0] == 'u') {
        peek = ReadCharacter(context); // consume 'u'
        // Expect 4 hex characters here
        for (size_t i = 0; i < 4; i++) {
          peek = PeekCharacter(context);
          if (!IsHexCharacter(peek)) {
            token.cursor_start = context.cursor - 1;
            token.cursor_end = context.cursor - 1;
            ReportLexerError(context, token, token, "Failed to parse unicode escape sequence",
                             "Expected hex character, instead got '" + peek + "'");
          }
          ReadCharacter(context); // consume hex character
        }
      } else {
        peek = ReadCharacter(context);
        if (peek[0] == 0x0A || peek[0] == EOF || peek == "") {
          token.cursor_start = context.cursor;
          token.cursor_end = context.cursor;
          ReportLexerError(context, token, token, "Failed to parse string", "Unterminated string");
        }
        token.literal += peek;
        continue;
      }
    }
    // check if peek terminates string literal
    // if not, save character in token literal
    peek = PeekCharacter(context);
    if (peek[0] != '"' && peek[0] != EOF) {
      peek = ReadCharacter(context);
      token.literal += peek;
      continue;
    }
    if (peek[0] == 0x0A || peek[0] == EOF || peek == "") {
      token.cursor_start = context.cursor + 1;
      token.cursor_end = context.cursor + 1;
      ReportLexerError(context, token, token, "Failed to parse string", "Unterminated string");
    }
    ReadCharacter(context);
    break;
  }
  token.cursor_end = token.cursor_start + token.literal.size() + 2;
  return token;
}

Token ReadNumber(Lexer &context, const std::string &character) {
  Token token{TokenType::NUMBER, character, context.filename, context.line, context.cursor};
  std::string next = ReadCharacter(context);
  std::string start = next;
  bool e_encountered = false;
  std::string prev = "";
  while (true) {
    auto peek = PeekCharacter(context);
    if (peek.size() == 1 &&
        (peek[0] == '.' ||
         // encountering an 'e' or 'E'
         // check that it's the first 'e' encountered
         // e.g., 1EE-5 is not valid
         (!e_encountered && (peek[0] == 'E' || peek[0] == 'e')) ||
         // handle scientific notation
         // If previous character was 'e' or 'E'
         // then the immediate next character is allowed to be
         // '-' or '+', e.g., e-2 E+10 etc.
         (e_encountered && prev.size() == 1 && (prev[0] == 'e' || prev[0] == 'E') &&
          (peek[0] == '-' || peek[0] == '+')) ||
         isdigit(peek[0]))) {
      if (peek[0] == 'e' || peek[0] == 'E')
        e_encountered = true;
      peek = ReadCharacter(context);
      token.literal += peek;
      prev = peek;
    } else {
      break;
    }
  }
  token.cursor_end = token.cursor_start + token.literal.size();
  return token;
}

bool IsIdentifier(const std::string &character) {
  if (character.size() == 0)
    return false;
  for (auto &c : character) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_') ||
        (c == '$') || ((unsigned char)c >= 0x80)) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

Token ReadIdentifier(Lexer &context) {
  Token token{TokenType::ILLEGAL, "", context.filename, context.line, context.cursor};
  std::string peek = "";
  while (true) {
    peek = PeekCharacter(context);
    if (IsIdentifier(peek)) {
      token.literal += peek;
      peek = ReadCharacter(context);
      continue;
    }
    break;
  }
  if (token.literal == "true")
    token.type = TokenType::TRUE;
  else if (token.literal == "false")
    token.type = TokenType::FALSE;
  else if (token.literal == "null")
    token.type = TokenType::NULL_;
  else {
    auto current = token;
    current.cursor_start = current.cursor_start;
    current.cursor_end = context.cursor;
    auto peek = current;
    peek.cursor_start = current.cursor_start + token.literal.size();
    ReportLexerError(context, current, peek, "Failed to parse keyword",
                     "Expected 'true', 'false', or 'null', instead got '" + token.literal + "'");
  }
  return token;
}

Token ReadPunctuation(Lexer &context, const std::string &character) {
  Token token{TokenType::ILLEGAL, character,      context.filename,
              context.line,       context.cursor, context.cursor + 1};
  auto next = ReadCharacter(context);
  if (next == ",") {
    token.type = TokenType::COMMA;
  } else if (next == ":") {
    token.type = TokenType::COLON;
  } else if (next == "+") {
    token.type = TokenType::PLUS;
  } else if (next == "-") {
    token.type = TokenType::MINUS;
  } else if (next == "[") {
    token.type = TokenType::LEFT_BRACKET;
  } else if (next == "{") {
    token.type = TokenType::LEFT_BRACE;
  } else if (next == "]") {
    token.type = TokenType::RIGHT_BRACKET;
  } else if (next == "}") {
    token.type = TokenType::RIGHT_BRACE;
  } else {
    token.cursor_start = context.cursor - 2;
    token.cursor_end = context.cursor - 1;
    ReportLexerError(context, token, token, "Failed to parse punctuation",
                     "Unexpected token '" + token.literal + "'");
  }
  return token;
}

void ReadWhitespace(Lexer &context) {
  // consume whitespace
  ReadCharacter(context);
  while (true) {
    auto peek = PeekCharacter(context);
    char c = '\0';
    if (peek.size() > 0)
      c = peek[0];
    if (c == 0x20 || c == 0x0D || c == 0x08 || c == 0x09) {
      ReadCharacter(context);
    } else
      return;
  }
}

} // namespace details

std::vector<Token> Tokenize(Lexer &context) {
  std::vector<Token> result;
  context.index = 0;
  using namespace details;
  while (context.index < context.source.size()) {
    if (IsUTF8(context.source[context.index])) {
      auto peek = PeekCharacter(context);
      if (isdigit(peek[0])) {
        result.push_back(ReadNumber(context, peek));
      } else if (peek[0] == '"') {
        result.push_back(ReadString(context));
      } else if (ispunct(peek[0])) {
        result.push_back(ReadPunctuation(context, peek));
      } else if (IsIdentifier(peek)) {
        result.push_back(ReadIdentifier(context));
      } else if (peek[0] == 0x20 || peek[0] == 0x0D || peek[0] == 0x08 || peek[0] == 0x09) {
        ReadWhitespace(context);
      }
      if (peek[0] == '\n') {
        context.line += 1;
        ReadCharacter(context);
        context.cursor = 1;
      }
      continue;
    }
  }
  Token token{TokenType::EOF_, "", context.filename, context.line, context.cursor, context.cursor};
  result.push_back(token);
  return result;
}

} // namespace jsonlint
