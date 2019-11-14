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
  Token token{TokenType::TK_STRING,"", context.filename, context.line, context.cursor, context.cursor};
  // consume first double quote
  ReadCharacter(context);
  while(true) {
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
      }
      else if (peek[0] == '\\') {
        next = ReadCharacter(context);
        // escaped backslash character
        token.literal += next;
      } else if (peek[0] == 'b' || // backspace
                peek[0] == 'f' || // form feed
                peek[0] == 'n' || // newline
                peek[0] == 'r' || // carriage return
                peek[0] == 't') {// horizontal tab
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

} // namespace details

std::vector<Token> Tokenize(LexerState &context) {
  std::vector<Token> result;
  context.index = 0;
  while (context.index < context.source.size()) {
    // auto peek =
  }

  return result;
}

} // namespace jsonlint