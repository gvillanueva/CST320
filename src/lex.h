#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#define MAX_ID_LENGTH   32 ///Identifier length restriction
#define OUTPUT_WIDTH    12 ///Formats output, pads lexemes to 12 chars

///Lexical Analyzer States
enum LEX_STATE {START = 0,

                PREPROCESSOR,
                PREPROCESSOR_DEFINE,
                PREPROCESSOR_MACRO,
                PREPROCESSOR_LOCAL_INCLUDE,
                PREPROCESSOR_EXCLUDE,

                OP_ADD,
                OP_SUB,
                OP_DIV,
                OP_AND,
                OP_OR,
                OP_LEFT,
                OP_RIGHT,
                OP_NOT,
                OP_ASSIGN,

                COMMENTS_SINGLE_LINE,

                KEYWORD_OR_ID,
                ID,
                CONSTANT,

                RUN_TO_ENDLINE};

// Forward declaration of Token class
class Token;

/***********************************************************************************
* Class: Lex
*
* Constructors:
*   Lex()
*       Initializes Lexical Analyzer by adding C- keywords to the Symbol Table.
*
* Mutators:
*   ~Lex()
*       Default destructor, included for sanity's sake.
*
* Methods:
*   Analyze()
*       Engine for the lexical analyzer. Reads character by character to determine
*       token eligibility for the C- programming language. If token is recognized,
*       outputs lexeme and token to STDOUT. If token is unrecognized, an error
*       message is written to STDOUT.
***********************************************************************************/
class Lex
{
    public:
        Lex(); //Constructor, pushes C- keywords onto Symbol Table
        ~Lex(); //destructor, does nothing
        int Analyze(const char *filename); //Recognizes tokens, generates output

    private:
        std::map<std::string, std::string> SymbolTable; //Holds data about keywords
        std::vector<std::string> IncludeStack; //Prevents infinite file recursion

        std::vector<Token*> m_Tokens;
};

#endif//LEX_H
