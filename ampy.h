#include <string>
#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, std::string> key_words_map;
key_words_map key_words;


void initialize_key_word_map();

bool verify_file(std::string file_name);