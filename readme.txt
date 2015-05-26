CST320 - Lab 2a
---------------
This lab implements a simple recursive descent parser for the LLC language.

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
1. g++ -o lab2a src/lex.cpp src/preprocessor.cpp src/syntaxanalyzer.cpp src/symbol.cpp src/symboltable.cpp src/token.cpp src/tokenlist.cpp tests/main.cpp
2. ../lab2a <filename>

If your source file includes a local include file, be sure to run the program
from within the same directory as your source and include file.

Expected output is nothing for a successful parse. If an error is encountered,
the program reports a syntax erro exists somewhere.

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
    src/syntaxanalyzer.h
    src/syntaxanalyzer.cpp
    src/token.h
    src/token.cpp
    src/tokenlist.h
    src/tokenlist.cpp
    tests/main.cpp
2. Compile and run.


Files
=====
llc-adjusted.txt	- An adjusted CFG for the LLC language.
readme.txt		- This readme file.
src/lex.cpp		- The implementation of the Lex class.
src/lex.h		- The header file of the Lex class.
src/preprocessor.h      - The header file of the preprocessor class.
src/preprocessor.cpp    - The implementation of the preprocessor class.
src/symbol.h            - The header file of the symbol class.
src/symbol.cpp          - The implementation of the symbol class.
src/symboltable.h       - The header file of the symbol table class.
src/symboltable.cpp     - The implementation of the symbol table class.
src/syntaxanalyzer.h    - The header file of the Syntax Analyzer class.
src/syntaxanalyzer.cpp  - The implementation of the Syntax Analyzer class.
src/token.h             - The header file of the token class.
src/token.cpp           - The implementation of the token class.
src/tokenlist.h         - The header file of the token list class.
src/tokenlist.cpp       - The implementation file of the token list class.

tests/main.cpp		- Contains int main and begins the lexical analyzer.
tests/sources/globals.h		- A file included within the valid test file "test1.cpp".
tests/sources/test1.cpp		- A valid test file.
tests/sources/test2.cpp		- An invalid test file, contains illegal identifiers and symbols.
tests/sources/test3.c		- A valid test file.
tests/sources/test_ifdef.cpp    - A valid test file for the #ifdef directive.
tests/sources/test_ifndef.cpp   - A valid test file for the #ifndef directive.

tests/test_lex.h        - A collection of simple tests for the lexical analyzer.
tests/test_lex.cpp
tests/test_token.h      - A collection of simple tests for the token class.
tests/test_token.cpp
tests/test_symboltable.h    - A collection of simple tests for the SymbolTable
tests/test_symboltable.cpp    class.
tests/test_syntaxanalyzer.h	- A collection of simple tests for the
tests/test_syntaxanalyzer.cpp	   SyntaxAnalyzer class