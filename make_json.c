#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *json_src;
    if (argc != 2){
        fprintf(stderr, "Only input one argument which is the file name");
        exit(1);
    }


    const char *file_name = argv[1];

    json_src = fopen(file_name, "r");
    
    if (!json_src){
        fprintf(stderr, "Can not open file");
        exit(1);
    }

    char c;
    int dquote_count = 0, comma_count = 0;

    FILE *json_output;
    json_output = fopen("keywords_parsed.json", "w");

    if (!json_output){
        fprintf(stderr, "Can not create json file");
        exit(1);
    }

    while ((c = fgetc(json_src)) != EOF){
        switch(c){
            case ',':
                comma_count ++;
                goto other;
            case '\"':
                dquote_count ++;
                goto other;

            case '\n':
                // fputc(c, json_output);
                dquote_count = 0, comma_count = 0;
                break;

            default:
            other:
                fputc(c, json_output);
                if (comma_count == 1 && dquote_count == 4){
                    fputc('\n', json_output);
                    dquote_count = 0, comma_count = 0;
                }
                break;
        }
    }

    return 0;
}