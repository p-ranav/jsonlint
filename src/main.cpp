#include <jsonlint/parser.hpp>
#include <argparse/argparse.hpp>
#include <fstream>
using namespace jsonlint;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("jsonlint");
  program.add_argument("file")
    .help("json file to validate");
  try {
    program.parse_args(argc, argv);
  }
  catch (const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
    std::cout << program;
    exit(0);
  }
  auto filename = program.get<std::string>("file");
  std::string source = "";
  try {
    std::ifstream stream(filename, std::ifstream::in);
    source =
        std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
  } catch (std::exception) {
    std::cerr << "error: failed to open " << filename << std::endl;
  }
  Lexer lexer{source, 0, filename, 1, 1};
  auto tokens = Tokenize(lexer);  
  Parser parser(tokens, source);
  parser.ParseJson();
  return 0;
}
