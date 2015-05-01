#ifndef LEX_H
#define LEX_H

#include <iostream>

#define MAX_ID_LENGTH   32 ///Identifier length restriction
#define OUTPUT_WIDTH    12 ///Formats output, pads lexemes to 12 chars

///Lexical Analyzer States
enum LEX_STATE {START = 0,

                PREPROCESSOR,
//                PREPROCESSOR_DEFINE,
//                PREPROCESSOR_MACRO,
//                PREPROCESSOR_LOCAL_INCLUDE,
//                PREPROCESSOR_EXCLUDE,

                OP_ADD,
                OP_SUB,
                OP_MUL,
                OP_DIV,
                OP_MOD,
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

                STRING,
                STRING_ESCAPE,

                RUN_TO_ENDLINE};

/// Forward declaration of Token class
class TokenList;
class SymbolTable;

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
    // Get token

    public:
        Lex(const SymbolTable &symbolTable); //Constructor, pushes C- keywords onto Symbol Table
        TokenList *Analyze(std::istream &istream, const char *filename=0); //Recognizes tokens, generates output
        TokenList *tokenizeString(const char *input);
        TokenList *tokenizeFile(const char *filename);

    private:
        const SymbolTable &m_SymbolTable;
};

#endif//LEX_H
