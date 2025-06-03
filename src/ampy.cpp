// clang++ --std=c++20 -stdlib=libc++ ampy.cpp utilities.cpp -o ampy
#include "ampy.hpp"
#include <cstdlib>

class Transpiler 
{
private:
  builtin_keywords file_keywords;
  std::filesystem::path path;
  std::ifstream input_file;
  std::ofstream output_file;
  bool hadError;

  void handle_import(bool ampy) {
    // handle the imports here
  }
  
  void handle_string(std::string *str) {
    bool ampy = false;
    if (str->size() > 0) {
      ampy = file_keywords.find(*str) != file_keywords.end();
      if (ampy) {
        *str = file_keywords[*str];
      }
      output_file.write(str->c_str(), str->length());
    }

    if (*str == "import") {
      handle_import(ampy);
    }

    *str = "";
  }

  bool handle_comment() {
    char c {'#'};
    while (c != '\n' && c != EOF) {
      output_file.put(c);
      c = input_file.get();
    }

    if (c == '\n') output_file.put(c);
    return c == EOF;
  }

  /*
    One quote has already been consumed.
  */
  bool is_triple_quote(char quote) {
    bool triple = false;
    // keep track of where in the file it is
    std::ifstream::pos_type pos {input_file.tellg()};
    if (input_file.peek() == quote) {
      input_file.get();
      triple = input_file.peek() == quote;
    }
    // revert back to where in the file it was
    input_file.seekg(pos);
    return triple;
  }

  bool hanlde_quotes(char quote, bool fstring) {
    bool triple {is_triple_quote(quote)};
    output_file.put(quote);

    if (triple) {
      output_file.put(input_file.get());
      output_file.put(input_file.get());
    }

    char c {};
    while ((c = input_file.get()) != EOF) {
      // if we see a quote and we are in a triple quote string,
      // check if this sequence will be closing the triple quote
      output_file.put(c);
      if (c != quote) continue;
      else if (!triple) break;
      // we are in triple quotes and saw the start of a potential closing
      else if (is_triple_quote(quote)) { 
        output_file.put(input_file.get());
        output_file.put(input_file.get());
        break;
      } 
    }
    return c == EOF;
  }

public:
  Transpiler(
    std::string arg
  ):
    file_keywords (default_keywords),
    path (arg)
  {
    input_file.open(arg);
    if (!input_file.is_open()) {
      std::cout << "Error with opening file " << arg << "\n";
      exit(1);
    }

    std::filesystem::path path {arg}, ext {".py"};
    output_file.open(path.replace_extension(ext));
    if (!output_file.is_open()) {
      std::cout << "Error createing output file " << path.replace_extension(ext) << "\n";
      exit(1);
    }

    hadError = false;
  };

  void transpile(){
    char c;
    std::string cur {};

    while ((c = input_file.get()) != EOF) {
      switch (c) {
      // strings between quotes "...",'...',"""...""",'''...'''
      // including fstrings
        case '\'': case '\"': {// quotes  
          bool is_fstring = cur == "f";
          handle_string(&cur);
          bool finished = hanlde_quotes(c, is_fstring);
          if (finished) break;
          break;
        }
        // end of words/strings
        case '(': case '[': case ']': case ')': case '{': case '}': // braces
        case '<': case '>': case '=': case '!': // comparison
        case '.': case ':': case '?': case ',': // punctionation
        case '|': case '@': case '*': case '%': case '&': case '^': case '+': case '-': // operations
        case '$': case '~': case '`': case ';': // other symbols
        case '\t': case ' ': case '\n': {// cases that can be associated with imports
          handle_string(&cur);
          output_file.put(c);
          break;
        }
        // comment
        case '#': {
          handle_string(&cur);
          bool finished = handle_comment();
          if (finished) break;
          break;
        }
        // aphanumeric: imports
        default: {
          cur += c;
          break;
        }
      }
    }

    handle_string(&cur);
    output_file.close();
    input_file.close();
  }

  void runFile(std::string filename) {
    // read all the bytes from the file
    // run(string)
    if (hadError) std::exit(65);
    
  }

  void run() {
    std::string cmd {"python3 " + path.replace_extension(".py").string()};
    int ret_code = system(cmd.c_str());
    exit(ret_code);
  }

  void runPrompt() {
    for (;;) {
      std::cout << "> ";
      std::string line {};
      std::cin >> line;

      if (std::cin.eof()) break;
        
      hadError = false;
      
    }
  }

};


int main(int argc, char *argv[]) {

  if (argc > 3) {
    std::exit(64);
  }
  read_keys_from_bin(default_keywords, default_library_keywords);

  if (argc == 3) {
    std::string arg(argv[1]);
    std::string file(argv[2]);

    if (arg == "test") {
      assert(default_keywords["ከሆነ"] == "if");
      assert(default_library_keywords["ባየሽ"]["ባየሽ"] == "random");
      assert(default_library_keywords["ጊዜ"]["ጊዜ"] == "time");
      exit(0);
    } 


    Transpiler transpiler = Transpiler(file);
    if (arg == "build") transpiler.transpile();

    if (arg == "run") {
      transpiler.transpile();
      transpiler.run();
    }

    // runFile(arg);
  } else {
    // runPrompt();
  }

  return 0;
}
