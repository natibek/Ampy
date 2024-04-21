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
    key_words = {
        {"ከሆነ" , "if"}, {"አለበለዚያ", "else"}, {"ሌላ ከሆነ", "elif"} , 
        {"እያለ", "while"}, {"ለ", "for"}, {"ስበር", "break"}, {"ቀጥል", "continue"}, {"እለፍ", "pass"},
        {"ክፈት", "open"}, {"በ", "with"}, 
        {"አጣምር", "match"}, {"ክስተት", "case"}, {"ቀር", "default"},  
        {"አጥፋ", "del"}, {"ሰርዝ", "remove"}, 
        {"እና", "and"}, {"ወይም", "or"} , {"እውነት", "True"}, {"ሁስት", "False"}, {"ሁሉም", "all"}, {"አንዱም", "any"}, {"አይደለም", "not"},
        {"እንደ", "as"}, {"አምጣ", "import"}, {"ከ", "from"},
        {"ግለፅ", "def"} ,{"ግለጽ", "def"}, {"መልስ", "return"}, {"አምርት", "yield"},
        {"ምክር", "try"}, {"", "except"}, {"መጨረሻ", "finally"}, {"አንሳ", "raise"},
        {"አጠቃላይ", "global"}, {"የውጪ", "nonlocal"},
        {"ነው", "is"}, {"ውስጥ", "in"}, {"ላምዳ", "lambda"} , {"ምንም", "None"}, 
        {"ጻፍ", "print"}, {"ፃፍ", "print"},
        {"፣", ","},  

        // class 
        {"ክፍል", "class"}, {"እራስ", "self"}, {"ባህሪ", "property"}, {"ቀያሪ", "setter"},
        {"", "iter"}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
        {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
        
        // string function
        {"አቀላቅል", "join"}, {"", "input"}, {"", "split"}, 
        {"", "endswith"}, {"", "capitalize"}, {"", "casefold"},
        {"", "center"}, {"", "encode"}, {"", "expandtabs"}, {"", "format"}, {"", "format_map"},
        {"", "isalnum"}, {"", "isalpha"}, {"", "isascii"}, {"", "isdecimal"}, {"", "isdigit"},
        {"", "ididentifier"}, {"", "islower"}, {"", "isnumeric"}, {"", "isprintable"}, {"", "isspace"},
        {"", "istitle"}, {"", "isupper"}, {"", "ljust"}, {"", "lower"}, {"", "lstrip"},
        {"", "maketrans"}, {"", "partition"}, {"", "replace"}, {"", "rfind"}, {"", "rindex"},
        {"", "rjust"}, {"", "rstrip"}, {"", "swapcase"}, {"", "title"}, {"", "translate"},
        {"", "upper"}, {"", "zfill"},


        // types
        {"", "str"}, {"", "int"}, {"", "float"}, {"", "complex"},
        {"", "dict"}, {"", "list"}, {"", "tuple"}, {"", "range"},
        {"", "set"}, {"", "bool"}, {"", "byte"}, {"", "bytearray"},
        {"", "frozenset"}, {"", "memoryview"}, 
        
        // common libraries
        {"", ""}, {"", "random"},

        // random module
        {"", "random"}, {"", "seed"}, {"ሁኔታውን-አምጣ", "getstate"}, {"ሁኔታውን-ቀይር", "setstate"}, 
        {"", "getrandbits"},{"", "randrange"}, {"", "randint"}, {"", "choice"}, {"", "choices"}, 
        {"ፐውዝ", "shuffle"}, {"", "sample"}, {"", "uniform"}, {"", "triangular"}, {"", "betavariate"}, {"", "expovariate"},
        {"", "gammavariate"}, {"", "gauss"}, {"", "lognovariate"}, {"", "normalvariate"}, {"", "vonmisesvariate"},
        {"", "paretovariate"}, {"", "weibulvariate"}, 

        // requests

        {"", "requests"}, {"", "head"}, {"", "patch"}, {"", "post"}, {"", "put"},
        {"", "request"}, 

        // Math
        {"", "min"}, {"", "max"}, {"", "abs"}, {"", "math"},
        {"", "sqrt"}, {"", "ceil"}, {"", "floor"}, {"", "asin"},
        {"", "acos"}, {"", "cos"},  {"", "sin"}, {"", "atan"},
        {"", "atan2"}, {"", "comb"},
        {"", "degrees"}, {"", "dist"}, {"", "exp"},
        {"", "fabs"}, {"", "factorial"}, {"", "floor"},{"", "gcd"}, {"", "tan"},
        {"", "hypo"}, {"", "isclose"}, {"", "isfinite"}, {"", "isinf"}, {"", "isnan"},
        {"", "issqrt"}, {"", "pow"}, {"", "radians"}, {"", ""}, {"", ""},
        

        // list functions
        {"", "sorted"}, {"", "sort"}, {"", "reversed"}, {"", "insert"},
        {"", "extend"}, {"", "append"}, {"", "clear"}, {"", "copy"}, {"", "count"}, {"", "index"}, 
        {"", "find"}, 

        // dict methods
        {"", "fromkeys"}, {"", "get"}, {"", "items"}, {"", "keys"}, {"", "pop"}, {"", "popitem"}, 
        {"", "setdefault"}, {"አድስ", "update"}, {"", "values"}, 
        
        // set methods
        {"", "add"}, {"", "difference"}, {"", "difference_update"}, {"ጣል", "discard"}, {"", ""}, {"", ""}, 
        {"", "intersection"}, {"", "instersection_update"}, {"", "isdisjoint"}, {"", "issubset"}, 
        {"", "issuperset"}, {"", "union"}, {"", "Union"}, {"ትይዩ-ልዩነት", "symmertic_difference"}, 
        {"ትይዩ-ልዩነት-አድስ", "symmertric_difference_update"}, 

        // file methods
        {"", "close"}, {"", "detach"}, {"", "fileno"}, {"", "flush"}, {"", "isatty"},
        {"", "read"}, {"", "readable"}, {"", "readline"}, {"", "readlines"}, {"", "seek"},
        {"", "seekable"}, {"", "tell"}, {"", "truncate"}, {"", "writable"}, {"", "write"},
        {"", "writelines"}, 
        
        // statistics
        {"", "statistics"}, {"", "harmonic_mean"}, {"", "mean"}, {"", "median"},
        {"", "median_grouped"}, {"", "median_high"}, {"", "median_low"}, {"", "mode"}, {"", "pstdev"},
        {"", "stdev"}, {"", "pvariance"}, {"", "variance"}, 
        
        {"", ""}, {"", ""},
        {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
        {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},

    };
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
