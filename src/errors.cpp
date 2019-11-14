#include <jsonlint/errors.hpp>
#include <fstream>

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

bool GetLines(std::string filename, std::vector<std::string> & result) {
	std::ifstream stream(filename.c_str());
 	if (!stream) {
		std::cerr << "Cannot open the File : " << filename << std::endl;
		return false;
	}
	std::string line;
	// Read the next line from File untill it reaches the end.
	while (std::getline(stream, line)) {
		// Line contains string of length > 0 then save it in vector
		if(line.size() > 0)
			result.push_back(line);
	}
	stream.close();
	return true;
}

template <typename Stage>
void ReportError(Stage &context, Token start, Token end, const std::string &brief_description,
                 const std::string &detailed_description) {
  context.errors.push_back(std::make_tuple(start, end, brief_description, detailed_description));

  std::string file = start.filename;
  unsigned int line = start.line;
  unsigned int cursor = start.cursor_start + 1;

  std::string what = file + ":" + std::to_string(line) + ":" + std::to_string(cursor) + ": " +
                     brief_description + ". " + detailed_description;

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

  std::cout << start.cursor_end << " " << end.cursor_start << std::endl;

  if (start.cursor_start == end.cursor_start && start.cursor_end == end.cursor_end) {
    // start and end are the same token
    message_carets = " ^ ";
  }
  else if (end.cursor_start - start.cursor_end == 1) {
    message_carets = " " + std::string(start.cursor_end - start.cursor_start, '^') + " ";
  } else if (end.cursor_start - start.cursor_end > 1) {
    message_carets = " " + std::string(end.cursor_start - cursor - 1, '^') + " ";
  } else {
    message_carets = " " + std::string(start.cursor_end - start.cursor_start, '^') + " ";
  }

  if (message_carets == " " || message_carets == "  ") {
    message_carets = " ^ ";
  }
  std::vector<std::string> lines;
  if (start.filename != "") {
    auto return_val = GetLines(start.filename, lines);
    if (!return_val) {    
      throw std::runtime_error(what);
      return;
    }
  } else {
    // Used for testing
    // Split context.source on \n
    lines = string::Split(context.source, "\n");
  }

  std::string error_line = lines[line - 1];

  std::cout << termcolor::red << termcolor::bold << "error: " << brief_description
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

void ReportLexerError(Lexer &context, Token start, Token end, const std::string &brief_description,
                      const std::string &detailed_description) {
  ReportError<Lexer>(context, start, end, brief_description, detailed_description);
}

void ReportParserError(Parser &context, Token start, Token end,
                       const std::string &brief_description,
                       const std::string &detailed_description) {
  ReportError<Parser>(context, start, end, brief_description, detailed_description);
}

} // namespace details

} // namespace jsonlint
