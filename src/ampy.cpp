// clang++ --std=c++20 -stdlib=libc++ ampy.cpp utilities.cpp -o ampy
#include "ampy.hpp"

class Transpiler 
{
private:
  builtin_keywords file_keywords;

public:
  Transpiler(): file_keywords (default_keywords) {
    std::cout << "Constructor called\n";
  };

  void run() {
    std::cout << "RUNNING\n";
  }

  void transpile(){
    std::cout << "Transpiler called\n";

    // imports for ampy should have a '.ampy' or '.am'
  }

};


int main(int argc, char *argv[]) {
  read_keys_from_bin(default_keywords, default_library_keywords);

  if (argc > 1) {
    std::string arg(argv[1]);
    if (arg == "test") {
      assert(default_keywords["ከሆነ"] == "if");
      assert(default_library_keywords["ባየሽ"]["ባየሽ"] == "random");
      assert(default_library_keywords["ጊዜ"]["ጊዜ"] == "time");
    }
  }
  
  Transpiler transpiler;
  transpiler.transpile();

  return 0;
}
