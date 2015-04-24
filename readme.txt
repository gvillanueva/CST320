CST320 - Lab 1b
---------------
This lab implements a simple lexical analyzer for the LLC language.

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
1. g++ -o lab1b src/lex.cpp src/main.cpp
2. cd tests
3. ../lab1b <filename>

Microsoft Visual C
~~~~~~~~~~~~~~~~~~
In Visual Studio
1. Add all files to a Visual C project in Visual Studio.
2. Compile and run.


Files
=====
readme.txt		- This readme file.
src/lex.cpp		- The implementation of the Lex class.
src/lex.h		- The header file of the Lex class.
src/main.cpp		- Contains int main and begins the lexical analyzer.
tests/test1.cpp		- A valid test file.
tests/test2.cpp		- An invalid test file, contains illegal identifiers and symbols.
tests/globals.h		- A file included within the valid test file "test1.cpp".
lex.txt			- Output from the lexical analyzer.

NOTE:
Ignore the #ifndef directives within the Lexical Analyzer class. The purpose
of these is simply for code folding and logical organization.