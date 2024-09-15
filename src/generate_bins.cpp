#include <fstream>
#include <iostream>
#include <json/json.h>

void generate_builtin_key_bin() {
  std::ofstream bin_out("./../data/builtin_keywords.bin",
                        std::ios_base::binary);

  std::ifstream json_file("./../data/builtin_keywords.json",
                          std::ifstream::binary);
  Json::Value myJson;
  json_file >> myJson;

  // std::cout << myJson << '\n';
  std::cout << myJson.size() << " : BUILTIN KEYWORDS JSON SIZE\n";

  std::string map_size = std::to_string(myJson.size());
  bin_out.write(map_size.c_str(), map_size.length() + 1);

  for (Json::Value::const_iterator it = myJson.begin(); it != myJson.end();
       ++it) {
    bin_out.write(it.key().asString().c_str(),
                  it.key().asString().length() + 1);
    bin_out.write((*it).asString().c_str(), (*it).asString().length() + 1);
  }

  bin_out.close();
  json_file.close();
}

void generate_library_key_bin() {
  std::ofstream bin_out("./../data/library_keywords.bin",
                        std::ios_base::binary);

  std::ifstream json_file("./../data/library_keywords.json",
                          std::ifstream::binary);
  Json::Value myJson;
  json_file >> myJson;

  // std::cout << myJson << '\n';

  int total_size = 0;
  for (Json::Value::const_iterator it = myJson.begin(); it != myJson.end();
       ++it)
    total_size += (*it).size();

  std::cout << total_size << " : LIBRARY KEYWORDS JSON SIZE\n";

  std::string map_size = std::to_string(total_size);
  bin_out.write(map_size.c_str(), map_size.length() + 1);

  for (Json::Value::const_iterator it = myJson.begin(); it != myJson.end();
       ++it) {
    bin_out.write(it.key().asString().c_str(),
                  it.key().asString().length() + 1);
    auto inner_map = (*it);
    for (Json::Value::const_iterator inner_it = inner_map.begin();
         inner_it != inner_map.end(); ++inner_it) {
      bin_out.write(inner_it.key().asString().c_str(),
                    inner_it.key().asString().length() + 1);
      bin_out.write((*inner_it).asString().c_str(),
                    (*inner_it).asString().length() + 1);
    }
    bin_out.write("-1", 3);
  }

  bin_out.close();
  json_file.close();
}

int main() {
  generate_builtin_key_bin();
  generate_library_key_bin();
  return 0;
}
