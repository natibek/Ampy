#ifndef AMPY_HPP
#define AMPY_HPP

#include <unordered_map>
#include <string>
#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <unistd.h>

// The data structure for containing the keywords. Mirrors the json
typedef std::unordered_map<std::string, std::string> builtin_keywords;
typedef std::unordered_map<std::string, std::string> str_map;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> library_keywords;

static library_keywords default_library_keywords;
static builtin_keywords default_keywords;

// utilities.cpp
bool is_ampy(std::string file_name);
library_keywords read_library_keywords_from_bin(std::ifstream &library_bin_input);
builtin_keywords read_builtin_keywords_from_bin(std::ifstream &builtin_bin_input);
void read_keys_from_bin(builtin_keywords &builtin_map, library_keywords &library_map);

#endif


