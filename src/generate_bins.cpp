#include <iostream>
#include <filesystem>

key_words_map initialize_key_word_map()
{   
  std::ifstream json_file ("./../data/builtin_keywords.json", std::ifstream::binary);
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
    if (initialized_key_words.count(element.first) > 1) {
      std::cout << element.first << " more than once\n";
      exit(1);
    }
  }

  if (initialized_key_words.size() != myJson.size()){
    std::cout << "The key_word has " << initialized_key_words.size() << " while the input has " << myJson.size() << " elements" << '\n';
    exit(1);
  }
  return initialized_key_words;
}

void generate_builtin_key_bin(builtin_keywords builtin_map) {
  std::ofstream bin_out ("./../data/builtin_keywords.bin", std::ios_base::binary);

  std::cout << builtin_map.size() << " : BUILTIN MAP SIZE\n";

  std::string map_size = std::to_string(builtin_map.size());
  bin_out.write(map_size.c_str(), map_size.length() + 1);

  for (const auto &[key, value]: builtin_map) {
    bin_out.write(key.c_str(), key.length() + 1);
    bin_out.write(value.c_str(), value.length() + 1);
  }

  bin_out.close();
}


void generate_library_key_bin(library_keywords library_map) {
  std::ofstream bin_out ("./../data/library_keywords.bin", std::ios_base::binary);

  std::cout << library_map.size() << " : LIBRARY MAP SIZE\n";
  int total_size = 0;
  for (const auto &[lib, inner_map]: library_map) total_size += inner_map.size();


  std::string map_size = std::to_string(total_size);
  bin_out.write(map_size.c_str(), map_size.length() + 1);

  for (const auto &[lib, inner_map]: library_map) {
    bin_out.write(lib.c_str(), lib.length() + 1);
    for (const auto &[key, val]: inner_map) {
      bin_out.write(key.c_str(), key.length() + 1);
      bin_out.write(val.c_str(), val.length() + 1);
    }
    bin_out.write("-1", 3);
  }
}

int main() {
  builtin_keywords builtin_map = initialize__map();
  generate_builtin_key_bin(initialized_map);
  generate_library_key_bin(initialized_map);

  return 0
}
