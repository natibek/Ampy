#include <unordered_map>
#include <string>

// The data structure for containing the keywords. Mirrors the json
typedef std::unordered_map<std::string, std::string> builtin_keywords;
typedef std::unordered_map<std::string, std::string> str_map;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> library_keywords;

bool is_ampy(std::string file_name);
library_keywords read_library_keywords_from_bin(std::ifstream &library_bin_input);
builtin_keywords read_builtin_keywords_from_bin(std::ifstream &builtin_bin_input);
void read_keys_from_bin(builtin_keywords &builtin_map, library_keywords &library_map);
// class AbstractAmpy: 
//   protected:
//   virtual static builtin_keywords initialize_keyword_map() = 0; 




