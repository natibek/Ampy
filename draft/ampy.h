
typedef std::unordered_map<std::string, std::string> key_words_map;
key_words_map key_words;

typedef std::unordered_map<std::string, key_words_map> libraries_map;
libraries_map library_map;

// enum LIBTAG {
//     LIBRARY,
//     VALUE
// };

// union MAPVAL {
//     std::string val;
//     key_words_map key_map;
// };

// reads json and converts to an unordered map
key_words_map initialize_key_word_map();

// verify files are .ampy
bool verify_file(std::string file_name);

// writes the onordered map to a binary file
// first writes the size of the key map and then the content
int write_key_to_bin(key_words_map initialized_map);

// reads binary file and writes to unordered map
// first reads the size and then the content
// then compares the read size to how many pairs were read to verify correct reading
int read_key_from_bin();

// use unordered map to look for key word mapping to transpile to python
void transpile(std::ifstream *src, std::ofstream *output);

