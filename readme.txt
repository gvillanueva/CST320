CST320 - Lab 1c
---------------
This lab implements a simple lexical analyzer and preprocessor for the LLC
language.

Installation
============
No additional libraries should be required. Only a standard C++ compiler is
necessary to build the lab from source.

Compiling from source
=====================
In the project root, use the compiler of your choice to build the code. For
example:

GNU Compiler Collection (GCC) - g++, mingw
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
From a bash command prompt
1. g++ -o lab1c src/lex.cpp src/preprocessor.cpp src/symbol.cpp src/symboltable.cpp src/token.cpp src/tokenlist.cpp tests/main.cpp
2. ../lab1c <filename>

NOTE: tests/test_ifndef.cpp uses several preprocessor directives, and might be
the most interesting example.

Microsoft Visual C
~~~~~~~~~~~~~~~~~~
In Visual Studio
1. Add the following files to a Visual C project in Visual Studio:
    src/lex.h
    src/lex.cpp
    src/preprocessor.h
    src/preprocessor.cpp
    src/symbol.h
    src/symbol.cpp
    src/symboltable.h
    src/symboltable.cpp
    src/token.h
    src/token.cpp
    src/tokenlist.h
    src/tokenlist.cpp
    tests/main.cpp
2. Compile and run.


Files
=====
readme.txt		- This readme file.
globals.h		- A file included within the valid test file "test1.cpp".
src/lex.cpp		- The implementation of the Lex class.
src/lex.h		- The header file of the Lex class.
src/preprocessor.h      - The header file of the preprocessor class.
src/preprocessor.cpp    - The implementation of the preprocessor class.
src/symbol.h            - The header file of the symbol class.
src/symbol.cpp          - The implementation of the symbol class.
src/symboltable.h       - The header file of the symbol table class.
src/symboltable.cpp     - The implementation of the symbol table class.
src/token.h             - The header file of the token class.
src/token.cpp           - The implementation of the token class.
src/tokenlist.h         - The header file of the token list class.
src/tokenlist.cpp       - The implementation file of the token list class.

tests/main.cpp		- Contains int main and begins the lexical analyzer.
tests/test1.cpp		- A valid test file.
tests/test2.cpp		- An invalid test file, contains illegal identifiers and symbols.
tests/test_ifdef.cpp    - A valid test file for the #ifdef directive.
tests/test_ifndef.cpp   - A valid test file for the #ifndef directive.

tests/test_lex.h        - A collection of simple tests for the lexical analyzer.
tests/test_lex.cpp
tests/test_token.h      - A collection of simple tests for the token class.
tests/test_token.cpp
tests/test_symboltable.h    - A collection of simple tests for the SymbolTable
tests/test_symboltable.cpp    class.


NOTE:
Ignore the #ifndef directives within the Lexical Analyzer class. The purpose
of these is simply for code folding and logical organization.
