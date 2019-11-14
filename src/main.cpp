#include <argparse/argparse.hpp>
#include <fstream>
#include <jsonlint/parser.hpp>
#include <termcolor/termcolor.hpp>
using namespace jsonlint;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("jsonlint");
  program.add_argument("file").help("json file to validate");
  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cout << err.what() << std::endl;
    std::cout << program;
    exit(0);
  }
  auto filename = program.get<std::string>("file");
  std::string source = "";
  try {
    std::ifstream stream(filename, std::ifstream::in);
    if (stream.is_open()) {
      std::cout << "Parsing " << filename << std::endl;
      source =
          std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
      Lexer lexer{source, 0, filename, 1, 1};
      auto tokens = Tokenize(lexer);
      Parser parser(tokens, source);
      parser.ParseJson();
    } else {
      std::cerr << termcolor::red << termcolor::bold << "error: failed to open " << filename
                << std::endl;
    }
  } catch (std::runtime_error &e) {
  }
  return 0;
}
