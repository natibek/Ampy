#include "ampy.hpp"

bool is_ampy(std::string file_name) {
  /* Verifies if the file exists and has an .ampy extension. Exits with code 1
   * if neither is true.
   *
   * Args:
   *    file_name: The filename.
   *
   * Returns true if the function did not exit.
   */

  if (!std::filesystem::exists(file_name)) {
    std::cout << file_name << " file does not exist.\n";
    exit(1);
  }

  std::filesystem::path fpath = file_name;
  if (fpath.extension() != ".ampy") {
    std::cout << file_name << " is not a .ampy file.\n";
    exit(1);
  }

  std::ifstream original_file(file_name);
  if (!original_file.is_open()) {
    std::cout << "Error with file " << file_name << std::endl;
    exit(1);
  }

  std::filesystem::path path{file_name}, ext(".py");
  /*
   * DO THIS IN The function that called is_ampy
  std::ofstream output_file (path.replace_extension(ext));

  if (!output_file.is_open()){
    std::cout << "Error with creating the output file " << path << std::endl;
    exit(1);
  }
  */
  return true;
}

library_keywords
read_library_keywords_from_bin(std::ifstream &library_bin_input) {
  /* Reads the `library_keywords.bin` binary and uses it to fill the
   * `library_keywords` `unordered_map`. Compares the number of the keywords
   * read and the count stated in the file to check if the binary has been
   * manipulated. Also, makes sure that the last pair reading stops when a key
   * can't be read.
   *
   * Args:
   *    library_bin_input: The input filestream for the binary containing the
   * `Ampy` keywords for python library functions and keywords.
   *
   * Returns the generated `library_keywords`.
   */

  library_keywords library_map;
  std::string lib, key, val;
  str_map inner_map;

  int read_keyword_count = 0;
  std::string keywords_size = "-1";
  std::getline(library_bin_input, keywords_size, '\0');

  if (stoi(keywords_size) == -1) {
    std::cout << "\nERROR reading key words size from binary.\n";
    exit(1);
  }

  // -1 should be used to mark end of library. It would therefore, show the
  // start as well
  if (!std::getline(library_bin_input, lib, '\0')) {
    std::cout << "`library_keywords.bin` binary is corrupt.\n";
    exit(1);
  }

  while (std::getline(library_bin_input, key, '\0')) {
    if (key == "-1") {
      // a new library's keywords have been reached
      library_map[lib] = inner_map;
      if (!std::getline(library_bin_input, lib, '\0'))
        break;
      str_map inner_map;
    } else if (!std::getline(library_bin_input, val, '\0')) {
      std::cout << "`library_keywords.bin` binary is corrupt.\n";
      exit(1);
    } else {
      inner_map[key] = val;
      read_keyword_count++;
    }
  }

  if (stoi(keywords_size) != read_keyword_count) {
    std::cout << "\nERROR => Read " << read_keyword_count << " | Expected "
              << stoi(keywords_size) << '\n';
    std::cout << "Download the correct library keyword binary.";
    exit(1);
  }
  // std::cout << read_keyword_count
  //           << " pairs successfully extracted from `library_keywords.bin`.\n";

  return library_map;
}

builtin_keywords
read_builtin_keywords_from_bin(std::ifstream &builtin_bin_input) {
  /* Reads the `builtin_keywords.bin` binary and uses it to fill the
   * `builtin_keywords` `unordered_map`. Compares the number of the keywords
   * read and the count stated in the file to check if the binary has been
   * manipulated. Also, makes sure that the last pair reading stops when a key
   * can't be read.
   *
   * Args:
   *    builtin_bin_input: The input filestream for the binary containing the
   * `Ampy` keywords for python builtin functions and keywords.
   *
   * Returns the generated `builtin_keywords`.
   */

  builtin_keywords builtin_map;
  std::string key, val;
  int read_keyword_count = 0;

  std::string key_words_size = "-1";
  std::getline(builtin_bin_input, key_words_size, '\0');

  if (stoi(key_words_size) == -1) {
    std::cout << "\nERROR reading key words size from binary.\n";
    exit(1);
  }

  while (std::getline(builtin_bin_input, key, '\0')) {
    if (!std::getline(builtin_bin_input, val, '\0')) {
      std::cout << "`builtin_keywords.bin` binary is corrupt.\n";
      exit(1);
    }
    builtin_map[key] = val;
    read_keyword_count++;
  }

  if (stoi(key_words_size) != read_keyword_count) {
    std::cout << "\nERROR => Read " << read_keyword_count << " | Expected "
              << stoi(key_words_size) << '\n';
    std::cout << "Download the correct builtin keyword binary.";
    exit(1);
  }

  // std::cout << read_keyword_count
  //           << " pairs successfully extracted from `builtin_keywords.bin`.\n";
  return builtin_map;
}

void read_keys_from_bin(builtin_keywords &builtin_map,
                        library_keywords &library_map) {
  /* Function to help read both `builtin_keywords.bin` and
   * `library_keywords.bin` using their respective functions. Makes sure the
   * files can be accessed and creates input filestreams for them to be used by
   * the functions that generate the `unordered_map` for either.
   *
   * Args:
   *    builtin_map: Reference of a `builtin_keywords` `unordered_map` that will
   * be filled with what is read from the `builtin_keywords.bin` binary.
   *    library_map: Reference of a `library_keywords` `unordered_map` that will
   * be filled with what is read from the `library_keywords.bin` binary.
   */
  if (access("./../data/builtin_keywords.bin", F_OK) == -1) {
    std::cout << "Error accessing with the `builtin_keywords` binary\n";
    exit(1);
  }

  if (access("./../data/library_keywords.bin", F_OK) == -1) {
    std::cout << "Error accessing with the `library_keywords` binary\n";
    exit(1);
  }

  std::ifstream builtin_bin_input("./../data/builtin_keywords.bin",
                                  std::ios_base::binary);
  if (!builtin_bin_input.is_open()) {
    std::cout << "Error opening with the `builtin_keywords` binary\n";
    exit(1);
  }

  std::ifstream library_bin_input("./../data/library_keywords.bin",
                                  std::ios_base::binary);
  if (!library_bin_input.is_open()) {
    std::cout << "Error opening with the `library_keywords` binary\n";
    exit(1);
  }

  builtin_map = read_builtin_keywords_from_bin(builtin_bin_input);
  library_map = read_library_keywords_from_bin(library_bin_input);
}

void read_keys(bool test) {
  builtin_keywords builtin_map;
  library_keywords library_map;
  read_keys_from_bin(builtin_map, library_map);

  if (test) {
    assert(builtin_map["ከሆነ"] == "if");
    assert(library_map["ባየሽ"]["ባየሽ"] == "random");
    assert(library_map["ጊዜ"]["ጊዜ"] == "time");
    std::cout << "ከሆነ = " << builtin_map["ከሆነ"]
              << " ባየሽ = " << library_map["ባየሽ"]["ባየሽ"]
              << " ጊዜ = " << library_map["ጊዜ"]["ጊዜ"];
  }
}

// int main() { read_keys(true); }
