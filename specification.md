![Ampy logo](/ampylogo.png)

# Specification For Ampy

## Language Features

* Mix regular ***python*** and ***ampy*** when programming with ampy.
* Import and use selected libraries with translated keywords.
* Import and use custom ***ampy*** and ***py*** files
* Use basic python syntax as expected
* Custom language features like 
    * `?` = is None
    * `!?` = is not None
    * `++` = increment by one
    * `--` = decrement by one
* All imports are on the top of the ***ampy*** file.
* Has the option to generate the resulting ***python*** file or not.
* *Maybe have a terminal version like python* 

## Software Design Features

### Imports

* Recursively follow imports and transpile all ***ampy*** files
* Have an *end-of-import* keyword to show no more imports will be done??

### Keywords

* Store the translated keywords in a json file
    ```javascript
    {
        'default':
            {'keywords':
                {'ampyfunckeyword': 'pythonfunckeyword'}, ...
            },
            {'functions':
                {'ampyfunckeyword': 'pythonfunckeyword'}, ...
            },
            {'variables':
                {'ampyvarkeyword': 'pythonvarkeyword'}, ...
            },
        'library_name': 
            {'functions':
                {'ampyfunckeyword': 'pythonfunckeyword'}, ...
            },
            {'variables':
                {'ampyvarkeyword': 'pythonvarkeyword'}, ...
            },
        ...
    }
    ``` 
    * This is helpful in future documentation as well.
    * Default keywords contains the python reserved keywords.

#### Generating keyword binary

* Use **JSONCPP** to generate ordered map and write to binary file.
    1. Before the start of every library include ***lib***.
    1. Before every function keyword mapping include ***func***.
    1. Before every variable keyword mapping include ***var***.
    1. Include `\0` as a terminating character for the above signifiers and the all ***ampy*** and ***python*** keywords.
* Include the count and a hash of the keywords at the start (in that order)

#### Reading keyword binary

* Create a unordered map `keywords_map` (global variable) to replicate the JSON structure above.
* Use the signifiers from the previous section to correctly parse the binary.
* Create a hash and keep a count to compare with the read has. If different generate binary again from the JSON and read that.
* Have a `file_keywords` unordered map that will contain the keywords that are available to the current ampy file.
    
### Dealing with Imports

* `file_keywords` will by default contain ***python*** keywords.
* Add keywords from the supported (translated) libraries to the `file_keywords` whenever they are imported.
    * If the import formated as, (*these would be in ampy*) 
        * ```python
          import os
          from os import *
          ``` 
          include all of the translated keywords from os.
        * ```python
          from os import path
          ```
          include only the ***path*** keyword.
         
### Transpiler

The transpiler should be an class that upon instantiation will create a `file_keywords` that contains only the default ***python*** keywords.

* If the ***ampy*** keyword related to imports are found, call the import handler to first verify if it is an ***ampy*** file and then create a new instance of the transpiler to transpile the imported file. 
* The transpiler parses the ***ampy*** file and if it reaches end of a word (hits **( ) " ' \t [ ] { } < > = ! . : ? , | @ * % & ^ + - $ ~ char32 \n**)
    * checks if the word is in `file_keywords['keywords']` and replaces with the ***python*** keyword if found.
    * if not found, checks if the next character is a **(** and if it is, checks if the word is in `file_keywords['functions']` and replaces with the ***python*** keyword if found.
    * if the next chacter was not **(**, checks if the word is in `file_keywords['variables']` and replaces with the ***python*** keyword if found.
    * this subprocess will be contained within an inline function `translate` that will take an input file stream and writes the resulting word to an output file stream.
* The transpiler ignores text found after **#** and within **""" """, ''' ''', " ", ' '**.
    * f strings fromated as 
        ```python
        f"ampy is {cool}"
        ```
        will be handled by ignoring translation on all the text that is not within **{ }**.

### Ampy File Verification

Function that verifies if a file has the extension .ampy. Will be overloaded to also take a boolean argument which when true, the function will return the file name and makes sure the file name does not have injected commands that will cause an issue when calling `~$ python3 file.py`. 

* Will be used when calling `~$ ampy file` in the terminal to verify if the file has an ampy extension.
* Will be used when importing a file to check if it should be transpiled as well.

