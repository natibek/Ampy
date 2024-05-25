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
#include "ampy.h"


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

  std::string key_words_size;
  getline(bin_input, key_words_size, '\0');

  std::string key, val;

  while (1) {
    
    if (!getline(bin_input, key, '\0'))
      break;
    if (!getline(bin_input, val, '\0'))
      break;
    
    //    std::cout << key <<  " -> " << val << '\n';
    key_words[key] = val;
    read ++;
  }
  bin_input.close();


  if (stoi(key_words_size) != read) {
    std::cout << "\nERROR => Read " << read << " | Expected " << stoi(key_words_size) << '\n';
    exit(1);
  }
  
  std::cout << read << " : PAIRS READ.\n";
  
  return 0;
}


void transpile(std::ifstream *src, std::ofstream *output)
{
    // go through the words in src and replace with any one from the key
    // and write it to the output.

    // find a getword methods
    // fgetc until you hit : , . [ ] { }
        // none alphabet character other than -
        // 

    // if (isalpha('አ')){
    //     std::cout << 'alpha\n';
    // } else {
    //     std::cout << 'not alpha \n';
    // }


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

    -----
    */

   std::string cur = "";
   char c, quote;
   int single_quote = 0, dbl_quote = 0, count;
   while ((c = (*src).get()) != EOF) {

        if (c == '\'') single_quote++;
        else if (c == '\"') dbl_quote++;

        if (single_quote == 3 || dbl_quote == 3) {
            count = 0;   
            quote = c;
            (*output).put(c);
            
            while (count < 3) {
                c = (*src).get();
                if (c == EOF) return;
                if (c == quote) count++;
                (*output).put(c);
            }            

            if (single_quote == 3) single_quote = 0;
            else if (dbl_quote == 3) dbl_quote = 0;
            cur = "";
            continue;
        }
    
        switch(c) {
            case '\'': case '\"': // quotes  
            case ' ': case '\n': case '\t': // white spaces
            case '[': case ']': case '(': case ')': case '{': case '}': // braces
            case '<': case '>': case '=': case '!': // comparison
            case '.': case ':': case '?': case ',': // punctionation
            case '|': case '@': case '*': case '%': case '&': case '^': case '+': case '-': // operations
            case '$': case '~': case '`': case ';': // other symbols
                // have ran into an end of a word, check if the word is an ampy key term
                if (cur.size() == 0) {
                    (*output).put(c);
                    break;    
                }

                if (key_words.find(cur) != key_words.end()) {
                    (*output).write(key_words[cur].c_str(), key_words[cur].length());
                    // std::cout << "AMPY " << cur << key_words[cur] << '\n';
                } else {
                    (*output).write(cur.c_str(), cur.length());          
                    // std::cout << "REG " << cur << '\n';
                }

                (*output).put(c);
                cur = "";
                break;

            case '#': // comment case
                if (cur.size() > 0) {
                    if (key_words.find(cur) != key_words.end()) {
                        (*output).write(key_words[cur].c_str(), key_words[cur].length());
                        // std::cout << "AMPY " << cur << key_words[cur] << '\n';
                    } else {
                        (*output).write(cur.c_str(), cur.length());          
                        // std::cout << "REG " << cur << '\n';
                    }
                }
                cur = "";
                
                while (c != '\n' && c != EOF) {
                    // std::cout << "CHAR AFTER # " << c << " ";
                    (*output).put(c);
                    c = (*src).get();
                }
                // std::cout << '\n';

                if (c == '\n') (*output).put(c);
                if (c == EOF) return;
                break;

            default: // any other character, add to current word
                cur += c;
                // std::cout << "DEF " << cur << '\n';
                break;
        }   
   }

   if (cur.size() > 0) {
        if (key_words.find(cur) != key_words.end()) {
            (*output).write(key_words[cur].c_str(), key_words[cur].length());
            // std::cout << "AMPY " << cur << key_words[cur] << '\n';
        } else {
            (*output).write(cur.c_str(), cur.length());          
            // std::cout << "REG " << cur << '\n';
        }
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
