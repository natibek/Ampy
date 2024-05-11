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

void initialize_key_word_map()
{   
    std::ifstream json_file ("./ampy_keywords.json", std::ifstream::binary);
    Json::Value myJson;

    json_file >> myJson;
    std::cout << myJson.size() << '\n';
    int count = 0;

    for (Json::Value::const_iterator it = myJson.begin(); it != myJson.end(); ++it) {
        if (key_words.contains(it.key().asString())){
            std::cout << it.key().asString() << " = " << (*it).asString() << '\n';
        }
        key_words[it.key().asString()] = (*it).asString();
        count ++;
    }

    if (count != myJson.size()){
        std::cout << "The parse json has " << count << " while the input has " << myJson.size() << " elements" << '\n';
        exit(1);
    }

    std::cout << count << ": size of parse\n";
    count = 0;
    int key_count;
    for (auto& element : key_words){
        count ++;
        
        if((key_count = key_words.count(element.first)) > 1){
            std::cout << element.first << " more than once\n";
        }
    }
    
    std::cout << count << '\n';

    if (key_words.size() != myJson.size()){
        std::cout << "The key_word has " << count << " while the input has " << myJson.size() << " elements" << '\n';
        exit(1);
    }
    std::cout << key_words.at("ላምዳ") << '\n';
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


    // https://stackoverflow.com/questions/23082819/detect-unicode-character-in-string
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
    std::ofstream output_file (path.replace_extension(ext));
    
    if (!output_file.is_open()){
        std::cout << "Error with creating the output file " << path << std::endl;
        exit(1);
    }

    initialize_key_word_map();
    transpile(&original_file, &output_file);

    original_file.close();
    output_file.close();
    return 0;
}
