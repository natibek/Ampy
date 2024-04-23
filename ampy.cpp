#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "ampy.h"


bool verify_file(std::string file_name){
    
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
    key_words = {};
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

    original_file.close();
    output_file.close();
    return 0;
}
