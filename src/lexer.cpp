#include <jsonlint/lexer.hpp>
#include <jsonlint/utf8.hpp>

namespace jsonlint {

namespace details {

std::string ReadCharacter(LexerState &context, bool update_index = true) {
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

std::string PeekCharacter(LexerState &context) { return ReadCharacter(context, false); }

Token ReadString(LexerState &context) {
  Token token{TokenType::TK_STRING, "", context.filename, context.line, context.cursor,
              context.cursor};
  // consume first double quote
  ReadCharacter(context);
  while (true) {
    // peek at next character
    auto peek = PeekCharacter(context);
    // check if peek is the start of an escape sequence
    if (peek[0] == '\\') {
      // consume escape character
      auto next = ReadCharacter(context);
      // check what comes next
      auto peek = PeekCharacter(context);
      if (peek[0] == '"') {
        next = ReadCharacter(context);
        // escaped double quote character
        token.literal += next;
        continue;
      } else if (peek[0] == '\\') {
        next = ReadCharacter(context);
        // escaped backslash character
        token.literal += next;
      } else if (peek[0] == 'b' || // backspace
                 peek[0] == 'f' || // form feed
                 peek[0] == 'n' || // newline
                 peek[0] == 'r' || // carriage return
                 peek[0] == 't') { // horizontal tab
        next = ReadCharacter(context);
        token.literal += '\\' + next[0];
      } else if (peek[0] == 'u') {
        // TODO: parse unicode
      } else {
        next = ReadCharacter(context);
        if (next[0] == 0x0A || next[0] == EOF) {
          // TODO: report unterminated string
        }
        token.literal += next;
        continue;
      }
    }

    // check if peek terminates string literal
    // if not, save character in token literal
    peek = PeekCharacter(context);
    if (peek[0] != '"' && peek[0] != EOF) {
      token.literal += ReadCharacter(context);
    }
    if (peek[0] == 0x0A || peek[0] == EOF) {
      // TODO: throw formatted error
    }
    ReadCharacter(context);
    break;
  }
  token.cursor_end = token.cursor_start + token.literal.size() + 2;
  return token;
}

Token ReadNumber(LexerState &context, const std::string &character) {
  Token token{TokenType::TK_NUMBER, character,      context.filename,
              context.line,         context.cursor, context.cursor};
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
      next = ReadCharacter(context);
      token.literal += next;
      prev = next;
      continue;
    }
    break;
  }
  token.cursor_end = token.cursor_start + token.literal.size();
  return token;
}

Token ReadPunctuation(LexerState &context, const std::string &character) {
  Token token{TokenType::TK_ILLEGAL, character, context.filename, context.line, context.cursor};
  auto next = ReadCharacter(context);
  if (next == "[") {
    token.type = TokenType::TK_LEFT_BRACKET;
  } else if (next == "{") {
    token.type = TokenType::TK_LEFT_BRACE;
  } else if (next == "]") {
    token.type = TokenType::TK_RIGHT_BRACKET;
  } else if (next == "}") {
    token.type = TokenType::TK_RIGHT_BRACE;
  } else {
    // TODO: report error - illegal token
  }
  token.cursor_end = token.cursor_start + token.literal.size();
  return token;
}

void ReadWhitespace(LexerState &context) {
  // consume whitespace
  ReadCharacter(context);
  while (true) {
    auto peek = PeekCharacter(context);
    char c = '\0';
    if (peek.size() > 0)
      c = peek[0];
    if (c == 0x20 || c == 0x0D || c == 0x0A || c == 0x09) {
      ReadCharacter(context);
    } else
      return;
  }
}

} // namespace details

std::vector<Token> Tokenize(LexerState &context) {
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
      } else if (peek[0] == 0x20 || peek[0] == 0x0D || peek[0] == 0x0A || peek[0] == 0x09) {
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
  return result;
}

} // namespace jsonlint