ampy_build: ampy.cpp
	clang++ -std=c++20 -o ampy ampy.cpp -ljsoncpp

parse_build: make_json.c
	clang -o json_parser make_json.c