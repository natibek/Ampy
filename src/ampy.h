#include <unordered_map>
#include <string>

// The data structure for containing the keywords. Mirrors the json
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>> keyword_map;
