#include <type_traits>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <ctype.h>
#include <cstdio>
#include <jsoncpp/json/json.h>
#include "ampy.hpp"
#include "utilities.cpp"


bool verify_file(std::string file_name)
{
    
  if (!std::filesystem::exists(file_name)){
    std::cout << file_name << " file does not exist.\n";
    exit(1);
  }

  if (!file_name.ends_with(".ampy")){
    std::cout << file_name << " is not a .ampy file.\n";
    exit(1);
  }

  return true;
}

key_words_map initialize_key_word_map()
{   
  std::ifstream json_file ("./ampy_keywords.json", std::ifstream::binary);
  Json::Value myJson;

  json_file >> myJson;
  std::cout << myJson.size() << " : JSON SIZE\n";

  int count = 0;
  key_words_map initialized_key_words;

  for (Json::Value::const_iterator it = myJson.begin(); it != myJson.end(); ++it) {
    if (key_words.contains(it.key().asString())){
      std::cout << it.key().asString() << " = " << (*it).asString() << '\n';
    }
    initialized_key_words[it.key().asString()] = (*it).asString();
    count ++;  
  }

  if (count != myJson.size()){
    std::cout << "The parse json has " << count << " while the input has " << myJson.size() << " elements" << '\n';
    exit(1);
  }

  std::cout << count << " : KEY MAP SIZE\n";

  count = 0;
  for (auto& element : initialized_key_words){
    count ++;
        
    if (initialized_key_words.count(element.first) > 1)
      std::cout << element.first << " more than once\n";
  }

  if (initialized_key_words.size() != myJson.size()){
    std::cout << "The key_word has " << initialized_key_words.size() << " while the input has " << myJson.size() << " elements" << '\n';
    exit(1);
  }
  return initialized_key_words;
}


int write_key_to_bin(key_words_map initialized_map)
{
  std::ofstream bin_out ("./keywords.bin", std::ios_base::binary);
  int len;

  std::cout << initialized_map.size() << " : KEY MAP SIZE\n";

  std::string key_word_size = std::to_string(initialized_map.size());
  bin_out.write(key_word_size.c_str(), key_word_size.length() + 1);

  for (auto x: initialized_map) {
    bin_out.write(x.first.c_str(), x.first.length() + 1);
    bin_out.write(x.second.c_str(), x.second.length() + 1);
  }

  bin_out.close();

  return 0;
}

int read_key_from_bin()
{

  std::ifstream bin_input ("./keywords.bin", std::ios_base::binary);

  int read = 0;
  std::string key, val, lib;

  key_words_map *cur_map;
  cur_map = &key_words;

  std::string key_words_size = "-1";
  getline(bin_input, key_words_size, '\0');

  while (1) {

    if (!getline(bin_input, key, '\0')) {
      if (cur_map->size() > 0) library_map[lib] = (*cur_map);
      break;
    }  

    if (!getline(bin_input, val, '\0'))
      break;

    if (val == "library") {
      library_map[key] = key_words_map ();
      cur_map = &library_map[key];
	
      std::cout << "Library " << key << '\n';

    } else {
      (*cur_map)[key] = val;    
    }
    read ++;
  }
    
  bin_input.close();

  if (stoi(key_words_size) == -1) {
    std::cout << "\nERROR reading key words size from binary\n";
    exit(1);
  }

  if (stoi(key_words_size) != read) {
    std::cout << "\nERROR => Read " << read << " | Expected " << stoi(key_words_size) << '\n';
    exit(1);
  }

  std::cout << read << " : PAIRS READ.\n";

  return 0;
}


void transpile(std::ifstream *src, std::ofstream *output)
{
  /*
    when a new word is started

    1. hit any punctuation mark (make sure unicode is not within ranges)
    a. open paren (
    b. close paren )
    c. colon :
    d. comma ,
    e. <, >, =, !, @, 
        
    2. empty spaces
    a. tab,
    b. space,
    c. new line

    check if the word is in the umap, 
    true => write the replacement in the new file
    false => write the word in the new file
    -------
    
    if " is the character:
    check if the last char makes it an f string
    ------

    Don't read characters from comments
    a. if # go until new line
    b. if multiline go until closing
    
    ------
    If importing check if any are /ampy files and change them to py as well
    - check if from has been used, store it in a flag
    - if from has been used the next word is the library,
    - add the keywords to the dictionary
    - else if import is ever the keyword and from has not been set, the next word is the library
        
    case of os.path is strange. Will import os
    -----

    differentiate between expressions/variables, callables.
    -> callables are followed by parenthesis
    -> others are expressions.
    -> the translations should discriminate between the function and the expression
    -> maybe have umap<string, umap<string, string>> the first has key func or var and the 
    second is the keyword map
  */

  key_words_map my_key_words(key_words);
    
  std::string cur = "", lib;
  char c, quote;
  int single_quote = 0, dbl_quote = 0, count;

  bool fstring_flag = false;
  bool sgl_qt_flag = false, dbl_qt_flag = false;
  bool import_flag = false, from_flag = false;

  while ((c = src->get()) != EOF) {    
    dbl_qt_flag = c == '\"';    
    sgl_qt_flag = c == '\'';            

    if (c == '\'') single_quote++;
    else single_quote = 0;

    if (c == '\"') dbl_quote++;
    else dbl_quote = 0;

    if ((single_quote == 1 && !sgl_qt_flag) || (dbl_quote == 1 && dbl_qt_flag)) {
      if (cur.size() > 0) {
	if (key_words.contains(cur)) cur = key_words[cur];
	output->write(cur.c_str(), cur.size());
	cur = "";

	fstring_flag = cur == "f";
      }

      quote = single_quote == 1 ? '\'' : '\"';
      output->put(c);

      while ((c = src->get()) != quote && c != EOF) {
	output->put(c);

	// build string in {}
      }

      if (c == EOF) return;
      if (c == quote) output->put(c);
            
      if (quote == '\'') single_quote = 0;
      else if (quote == '\"') dbl_quote = 0;

      fstring_flag = false;
      continue;
    }

    if ((single_quote == 3)|| (dbl_quote == 3)){
      if (cur.size() > 0) {
	if (key_words.contains(cur)) cur = key_words[cur];
	output->write(cur.c_str(), cur.size());
	cur = "";
      }

      count = 0;   
      quote = c;
      output->put(c);
            
      while (count < 3) {
	c = src->get();
	if (c == EOF) return;
                
	if (c == quote) count++;
	else count = 0;

	output->put(c);
      }            

      sgl_qt_flag = false;
      dbl_qt_flag = false;
      single_quote = 0;
      dbl_quote = 0;
 
      continue;
    }
    
    switch(c) {
      // case '(': // constructed string is a function name or any other case where () is used for conditions or cleaning up code
      //     // look in the key_words["function"].contains(cur)
      //     if (cur.size() == 0) {
      //         output->put(c);
      //         break;
      //     }

      //     /*
      //     if (key_words["functions"].contains(cur)) {
      //         cur = key_words["functions"][cur];
      //     } else if (key_words["expressions"].contains(cur)) {
      //         cur = key_words["expressions"][cur];
      //     }
      //     output->write(cur);
      //     output->put(c);
      //     cur = "";
      //     */
      //     break;
    case '(':
    case '\'': case '\"': // quotes  
    case '\t': // white spaces
    case '[': case ']': case ')': case '{': case '}': // braces
    case '<': case '>': case '=': case '!': // comparison
    case '.': case ':': case '?': case ',': // punctionation
    case '|': case '@': case '*': case '%': case '&': case '^': case '+': case '-': // operations
    case '$': case '~': case '`': case ';': // other symbols

    case ' ': case '\n': // cases that can be associated with imports

      // have ran into an end of a word, check if the word is an ampy key term
                
                
      if (cur.size() == 0) {
	output->put(c);
	break;
	// if (c != '*' || (!import_flag && !from_flag)) break;    
      }
      std::cout << "HERE " << cur << "\n";
      if (key_words.contains(cur)) { //key_words.find(cur) != key_words.end()) {
	cur = key_words[cur];
	std::cout << "AMPY " << cur << key_words[cur] << '\n';
      }
                
      // if (import_flag && from_flag) { // case like "from math import sin" or "from math import *" cur = "sin" or c = "*", lib = "math"
      //     import_flag = false;
      //     from_flag = false;
                    
      //     if (c == '*') {
      //         if (library_map.contains(lib)) {
      //             my_key_words.merge(library_map[lib]);
      //         } else if (std::filesystem::exists(lib + ".ampy")){ // importing custom python or ampy file
      //         // check if file is ampy, then transpile that as well
      //         }
      //         break;
      //     } else if (library_map.contains(lib)) { 
      //     // case "form math import sin" cur = 'sin' sin will be translated in math but if it has translation for sub methods, 
      //     // then sin itself will have a library_map for it which will be added to key_words_map
      //         if (library_map.contains(cur)) {
      //             my_key_words.merge(library_map[cur]);
      //         } else if (library_map[lib].contains(cur)) {
      //             my_key_words[cur] = library_map[lib][cur];
      //             cur = library_map[lib][cur];
      //         } else if (std::filesystem::exists(lib + ".ampy")) { // custom library
      //             // check if lib is an ampy file and transpile it
      //             std::string dependency = lib + ".ampy";
      //             std::ifstream dependency_file (dependency);
      //             std::filesystem::path path {dependency}, ext (".py");
      //             std::string dst_file_name = (path.replace_extension(ext));
      //             std::ofstream output_file (dst_file_name);
      //             transpile(&dependency_file, &output_file);
      //         }
    
      //     }   
      // } else if (import_flag && !from_flag) { // case like "import math" and cur = "math" and import has been seen already
      //     import_flag = false;
      //     if (library_map.contains(cur)) {
      //         my_key_words.merge(library_map[cur]); // adds the translated words from the other library to the key_words
      //     } else if (std::filesystem::exists(lib + ".ampy")) { // importing custom python or ampy file
      //     // check if file is ampy, then transpile that as well
      //         std::string dependency = lib + ".ampy";
      //         std::ifstream dependency_file (dependency);
      //         std::filesystem::path path {dependency}, ext (".py");
      //         std::string dst_file_name = (path.replace_extension(ext));
      //         std::ofstream output_file (dst_file_name);
      //         transpile(&dependency_file, &output_file);
      //     }  
      // } else if (from_flag) { // case like "from math", cur = "math"
      //     lib = cur;
      // }

      // if (cur == "from") from_flag = true;
      // else if (cur == "import") import_flag = true;
                

      output->write(cur.c_str(), cur.length());          
      output->put(c);
      cur = "";

      if (c == '\n') { //consider the case where \ has been used to continue to the next line
	from_flag = false;
	import_flag = false;
      }
      break;


    case '#': // comment case
      std::cout << "COMMENT\n";
      if (cur.size() > 0) {
	if (key_words.contains(cur)) { //    if (key_words.find(cur) != key_words.end()) {
	  cur = my_key_words[cur];
	  // std::cout << "AMPY " << cur << key_words[cur] << '\n';
	}

	output->write(cur.c_str(), cur.length());          
	cur = "";
      }
                
                
      while (c != '\n' && c != EOF) {
	// std::cout << "CHAR AFTER # " << c << " ";
	output->put(c);
	c = src->get();
      }
      // std::cout << '\n';

      if (c == '\n') output->put(c);
      if (c == EOF) return;
      break;

    default: // any other character, add to current word
      cur += c;
      std::cout << "DEF " << cur << '\n';
      break;
    }   
  }

  if (cur.size() > 0) {
    if (key_words.contains(cur)) {  // if (key_words.find(cur) != key_words.end()) {
      cur = my_key_words[cur];
      std::cout << "AMPY " << cur << key_words[cur] << '\n';
    }

    output->write(cur.c_str(), cur.length());          
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2){
    std::cout << "No .ampy file chosen.\n";
    exit(1);
  }

  const std::string file_name = argv[1];
    
  if(!verify_file(file_name)){
    std::cout << "Error with file " << file_name << std::endl;
    exit(1);
  }
    
  std::ifstream original_file (file_name);
    
    
  if (!original_file.is_open()){
    std::cout << "Error with file " << file_name << std::endl;
    exit(1);
  }

  std::filesystem::path path {file_name}, ext (".py");
  std::string dst_file_name = (path.replace_extension(ext));
  std::ofstream output_file (dst_file_name);
    
  if (!output_file.is_open()){
    std::cout << "Error with creating the output file " << path << std::endl;
    exit(1);
  }

  if (access( "./keywords.bin", F_OK ) == -1 ){
    key_words_map initialized = initialize_key_word_map();
    std::cout << "\nFINISHED INITIALIZING FROM JSON \n\n";

    write_key_to_bin(initialized);
    std::cout << "\nFINISHED WRITING BINARY \n\n";
  }

  read_key_from_bin();
  std::cout << "\nFINISHED READING BINARY.\n\n";
  transpile(&original_file, &output_file);

  original_file.close();
  output_file.close();

  std::cout << "...........\n\n";

  std::string command = "python3 " + dst_file_name;   
  system(command.c_str());
  return 0;
}
