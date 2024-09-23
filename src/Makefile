ampy_build: ampy.cpp
	clang++ -std=c++20 -o ampy ampy.cpp 

gen_bins: generate_bins.cpp
	clang++ -std=c++20 -o gen_bins generate_bins.cpp -ljsoncpp

utilities: utilities.cpp
	clang++ -std=c++20 -o utils utilities.cpp

parse_build: make_json.c
	clang -o json_parser make_json.c
