#pragma once
#include <jsonlint/lexer.hpp>
#include <jsonlint/parser.hpp>
#include <jsonlint/string.hpp>
#include <string>
#include <termcolor/termcolor.hpp>

namespace jsonlint {

namespace details {

unsigned int GetNumberOfDigits(unsigned int number);

template <typename Stage>
void ReportError(Stage &context, Token start, Token end, const std::string &brief_description,
                 const std::string &detailed_description);

void ReportLexerError(Lexer &context, Token start, Token end, const std::string &brief_description,
                      const std::string &detailed_description);

void ReportParserError(Parser &context, Token start, Token end,
                       const std::string &brief_description,
                       const std::string &detailed_description);

} // namespace details

} // namespace jsonlint
