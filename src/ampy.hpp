#include <unordered_map>
#include <string>

// The data structure for containing the keywords. Mirrors the json
typedef std::unordered_map<std::string, std::string> builtin_keywords;
typedef std::unordered_map<std::string, std::string> str_map;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> libraries_keywords;

class AbstractAmpy: 
  protected:
  virtual static builtin_keywords initialize_keyword_map() = 0; 
  virtual




