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

void generate_builtin_key_bin(key_words_map initialized_map) {
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


void generate_library_key_bin() {

}

int main() {
  keyword_map initialized_map = initialize_key_word_map();
  generate_builtin_key_bin()
}
