#include "utilities.cpp"

int main(int argc, char *argv[]) {
  builtin_keywords builtin_map;
  library_keywords library_map;
  read_keys_from_bin(builtin_map, library_map);

  if (argc > 1) {
    std::string arg(argv[1]);
    if (arg == "test") {
      assert(builtin_map["ከሆነ"] == "if");
      assert(library_map["ባየሽ"]["ባየሽ"] == "random");
      assert(library_map["ጊዜ"]["ጊዜ"] == "time");
      std::cout << "ከሆነ = " << builtin_map["ከሆነ"]
                << " ባየሽ = " << library_map["ባየሽ"]["ባየሽ"]
                << " ጊዜ = " << library_map["ጊዜ"]["ጊዜ"];
    }
  }
}
