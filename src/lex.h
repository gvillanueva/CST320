/*!
 * \author      Giancarlo Villanueva
 * \date        Created, 4/21/2015
 *              Modified, 4/30/2015
 * \ingroup     CST320 - Lab1c
 * \file        lex.h
 *
 * \brief       Declares the structure of the Lex classes.
 */
#ifndef LEX_H
#define LEX_H

#include <iostream>

#define MAX_ID_LENGTH   32 ///Identifier length restriction
#define OUTPUT_WIDTH    12 ///Formats output, pads lexemes to 12 chars

///Lexical Analyzer States
enum LEX_STATE {START = 0,

                PREPROCESSOR,

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

/// Forward declarations
class TokenList;
class SymbolTable;

/*!
 * \brief The Lex class tokenizes an input file according to the LLC language.
 */
class Lex
{
public:
    /// Constructs a new Lex object.
    Lex(const SymbolTable &symbolTable);

    /// Tokenizes an input stream.
    TokenList *Analyze(std::istream &istream, const char *filename=0);

    /// Tokenizes a string.
    TokenList *tokenizeString(const char *input);

    /// Tokenizes a file.
    TokenList *tokenizeFile(const char *filename);

private:
    /// Reference to a constant SymbolTable object.
    const SymbolTable &m_SymbolTable;
};

#endif//LEX_H
